/*
 * A046522  a(n) = 2*floor(sqrt(n)) - d(n), where d(n) is the number of
 *          divisors of n (A000005).
 * https://oeis.org/A046522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(sqrt(n)) - d(n) (d(n) = number of divisors of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for idx_t (unsigned 64‑bit). */
static idx_t isqrt_idx(idx_t x)
{
    /* Highest power of 4 <= x */
    idx_t bit = 1ULL << ((sizeof(idx_t) * 8 - 1) & ~1);
    while (bit > x) bit >>= 2;

    idx_t res = 0;
    while (bit) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Cache for divisor counts. */
static seq_t *divcnt = NULL;
static idx_t divcnt_size = 0;

/* Ensure we have divisor counts up to at least 'n'. */
static void ensure_divcnt(idx_t n)
{
    if (n <= divcnt_size)
        return;

    /* Allocate a new array large enough. */
    seq_t *new_arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!new_arr)   /* allocation failure – abort (should not happen in tests) */
        return;

    /* Initialise all entries to 0. */
    for (idx_t i = 0; i <= n; ++i)
        new_arr[i] = 0;

    /* Classic divisor sieve – O(n log n). */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            ++new_arr[j];
    }

    /* Replace old cache. */
    if (divcnt)
        free(divcnt);
    divcnt = new_arr;
    divcnt_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Make sure divisor counts are available. */
    ensure_divcnt(n);

    /* Compute floor(sqrt(n)). */
    idx_t s = isqrt_idx(n);
    seq_t sqrt_floor = (seq_t)s;

    /* Retrieve divisor count d(n). */
    seq_t d = divcnt[n];

    /* a(n) = 2*floor(sqrt(n)) - d(n) */
    seq_t result = (seq_t)(2 * sqrt_floor - d);
    return result;
}
