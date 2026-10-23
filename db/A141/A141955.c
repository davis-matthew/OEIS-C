/*
 * A141955  Primes congruent to 11 mod 27.
 * https://oeis.org/A141955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p ≡ 11 (mod 27)
 * Range:     u8 n <= 3, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* static storage for the sieve of Eratosthenes */
static bool *s_is_prime = NULL;   /* is I prime? */
static idx_t s_limit = 0;         /* upper bound of current sieve */

/* static storage for primes ≡ 11 (mod 27) */
static seq_t *s_residue = NULL;           /* array of primes */
static size_t s_residue_cnt = 0;           /* number of stored primes */
static size_t s_residue_cap = 0;           /* allocated capacity */

/* next number to test in the current sieve window */
static idx_t s_last_checked = 2;

/* Rebuild the sieve up to 'limit' (inclusive).  All old data is discarded. */
static void sieve_to(idx_t limit)
{
    if (s_limit >= limit) return;           /* nothing to do */
    bool *new_arr = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_arr) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) new_arr[i] = true;
    if (limit >= 0) new_arr[0] = false;
    if (limit >= 1) new_arr[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (new_arr[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                new_arr[j] = false;
        }
    }
    if (s_is_prime) free(s_is_prime);
    s_is_prime = new_arr;
    s_limit    = limit;
}

/* Ensure that the residue array can hold at least 'needed' elements. */
static void ensure_residue_capacity(size_t needed)
{
    if (needed <= s_residue_cap) return;
    size_t new_cap = s_residue_cap ? (s_residue_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(s_residue, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    s_residue = tmp;
    s_residue_cap = new_cap;
}

/* Fill s_residue with at least n terms. */
static void fill_up_to(idx_t n)
{
    if (n == 0) return;            /* a(0) is undefined */
    if (s_residue == NULL) {
        s_residue_cap = 16;
        s_residue = (seq_t *)malloc(s_residue_cap * sizeof(seq_t));
        if (!s_residue) exit(EXIT_FAILURE);
    }

    while (s_residue_cnt < (size_t)n) {
        if (s_limit == 0) {
            /* Initial sieve big enough for n ≈ 10 000 */
            sieve_to(3000000);
            s_last_checked = 2;
        }
        /* Scan the current sieve window. */
        for (idx_t num = s_last_checked; num <= s_limit; ++num) {
            if (s_is_prime[num] && (num % 27ULL) == 11ULL) {
                if (s_residue_cnt + 1 > s_residue_cap)
                    ensure_residue_capacity(s_residue_cnt + 9);
                s_residue[s_residue_cnt++] = (seq_t)num;
            }
            if (s_residue_cnt >= (size_t)n) {
                s_last_checked = num + 1;
                return;
            }
        }
        /* Need more primes: double the sieve limit and continue. */
        s_last_checked = s_limit + 1;
        sieve_to(s_limit * 2);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    fill_up_to(n);
    return s_residue[n - 1];
}
