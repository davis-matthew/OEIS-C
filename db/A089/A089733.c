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
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
        seq_arr = mem_new(seq_t, seq_cap);
    }
    while (needed >= seq_cap) {
        seq_cap <<= 1;
        seq_arr = mem_resize(seq_t, seq_arr, seq_cap);
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
