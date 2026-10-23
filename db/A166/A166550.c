/*
 * A166550  Numbers n with property that n^2 and n-th prime have the same sum
 *          of digits.
 * https://oeis.org/A166550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166550: numbers n such that sumdigits(n²) == sumdigits(prime(n)) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/get_prime.h"

#define MAX_CACHE 200000   /* sufficient for n up to 10000 */

static seq_t  cache[MAX_CACHE];
static size_t cache_len = 0;    /* number of stored terms */
static idx_t  checked_up_to = 0;/* largest n that has been inspected */

/* Ensure that at least n terms are cached. */
static void ensure_terms(idx_t n)
{
    while (cache_len < n) {
        ++checked_up_to;                         /* examine next candidate */

        /* digit sum of the square */
        seq_t m_sq = (seq_t)checked_up_to * (seq_t)checked_up_to;
        seq_t d_sq = digit_sum(m_sq);

        /* digit sum of the m-th prime */
        seq_t prime  = (seq_t)get_prime(checked_up_to);
        seq_t d_prime = digit_sum(prime);

        if (d_sq == d_prime) {
            if (cache_len == MAX_CACHE) {
                /* capacity exceeded – should not happen for the target range */
                abort();
            }
            cache[cache_len++] = checked_up_to;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 as fallback */
    ensure_terms(n);
    return cache[n - 1];
}
