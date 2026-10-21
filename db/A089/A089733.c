/*
 * A089733  Begin with 1; add a distinct prime to get a composite number, then
 *          add a distinct composite number to get a prime; repeat.
 * https://oeis.org/A089733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1; for n>1 a(n)=a(n-1)+p, where p is the smallest unused
 *            prime that makes a(n) composite if n is odd, or the smallest
 *            unused composite that makes a(n) prime if n is even
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* Sieve limit: large enough for the first ~10000 terms */
#define SIEVE_LIMIT 2000000U

/* Prime sieve and usage flags (up to SIEVE_LIMIT) */
static bool *prime_sieve     = NULL;
static bool *used_prime_flag = NULL;
static bool *used_comp_flag  = NULL;

/* Dynamic array holding the sequence */
static seq_t *seq_arr  = NULL;
static idx_t seq_len   = 0;   /* number of stored terms; seq_arr[1] is a(1) */
static size_t seq_cap  = 0;

/* Initialise the sieve and flag arrays once */
static void init_sieve(void) {
    if (prime_sieve) return;       /* already initialised */

    prime_sieve     = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    used_prime_flag = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    used_comp_flag  = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) prime_sieve[i] = true;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (prime_sieve[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                prime_sieve[j] = false;
}

/* Ensure seq_arr has capacity for at least 'needed' elements */
static void ensure_seq_capacity(idx_t needed) {
    if (!seq_arr) {
        seq_cap = 128;
        seq_arr = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    }
    while (needed >= seq_cap) {
        seq_cap <<= 1;
        seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    }
}

/* Helper: check if a small number is unused prime */
static inline bool is_prime_unused(size_t x) {
    return x <= SIEVE_LIMIT && prime_sieve[x] && !used_prime_flag[x];
}

/* Helper: check if a small number is unused composite (≥4) */
static inline bool is_comp_unused(size_t x) {
    return x <= SIEVE_LIMIT && !prime_sieve[x] && x >= 4 && !used_comp_flag[x];
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined for n<1 */

    init_sieve();
    ensure_seq_capacity(n);

    /* initialise the sequence on first call */
    if (seq_len == 0) {
        seq_arr[1] = (seq_t)1;
        seq_len = 1;
    }
    if (seq_len >= n) return seq_arr[n];

    /* build sequence up to n */
    for (; seq_len < n; ++seq_len) {
        seq_t cur = seq_arr[seq_len];
        bool add_prime = (seq_len & 1ULL) ? true : false;   /* odd index → add prime */

        if (add_prime) {
            size_t cand = 2;
            for (;;) {
                seq_t sum = cur + (seq_t)cand;
                if ((cand <= SIEVE_LIMIT && is_prime_unused(cand)) ||
                    (cand > SIEVE_LIMIT && is_prime_big((seq_t)cand))) {
                    if (!is_prime_big(sum)) {   /* sum must be composite */
                        if (cand <= SIEVE_LIMIT) used_prime_flag[cand] = true;
                        seq_arr[seq_len + 1] = sum;
                        break;
                    }
                }
                ++cand;
            }
        } else { /* even index → add composite */
            size_t cand = 4;
            for (;;) {
                seq_t sum = cur + (seq_t)cand;
                if ((cand <= SIEVE_LIMIT && is_comp_unused(cand)) ||
                    (cand > SIEVE_LIMIT && !is_prime_big((seq_t)cand))) {
                    if (is_prime_big(sum)) {   /* sum must be prime */
                        if (cand <= SIEVE_LIMIT) used_comp_flag[cand] = true;
                        seq_arr[seq_len + 1] = sum;
                        break;
                    }
                }
                ++cand;
            }
        }
    }
    return seq_arr[n];
}
