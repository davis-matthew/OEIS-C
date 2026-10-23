/*
 * A126692  Prime numbers p such that 1000-p is also a prime. All terms are
 *          shown.
 * https://oeis.org/A126692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   p such that 1000-p is also prime (OEIS A126692)
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple primality test for numbers <= 1000 */
static inline bool is_prime_small(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Cached list of valid primes */
static seq_t *valid = NULL;
static size_t valid_cnt = 0;
static bool initialized = false;

/* Return the n-th term (1‑based) of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* index < offset, return 0 */

    if (!initialized) {
        /* We know at most 1000 primes, so array of 200 is safe */
        valid = (seq_t *)malloc(sizeof(seq_t) * 200);
        valid_cnt = 0;
        for (idx_t p = 2; p <= 1000; ++p) {
            if (is_prime_small(p) && is_prime_small(1000 - p)) {
                valid[valid_cnt++] = (seq_t)p;
            }
        }
        initialized = true;
    }

    if ((size_t)(n - 1) < valid_cnt)
        return valid[n - 1];
    else
        return 0;   /* out of bounds */
}
