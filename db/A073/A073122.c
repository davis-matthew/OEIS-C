/*
 * A073122  Minimal reversing binary representation of n. Converting sum of
 *          powers of 2 in binary representation of a(n) to alternating sum
 *          gives n. See A072339.
 * https://oeis.org/A073122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(2n) = 2*a(n). For any n that is not a power of two, let p be
 *            the smallest power of two strictly greater than n. Then a(n) = p
 *            + a(p - n). This recurrence generates the unique minimal
 *            reversing binary representation of n
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 998 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Recursive helper: computes a(n) for n >= 1. */
static seq_t a_internal(idx_t n)
{
    if (n == 0)         /* not used, but graceful for 0-index */
        return (seq_t)0;

    /* Check if n is a power of two: n & (n-1) == 0 */
    if ((n & (n - 1)) == 0)
        return (seq_t)n;        /* minimal representation of a power of two */

    /* Find the next power of two greater than n */
    idx_t p = 1;
    while (p <= n)              /* at most 64 iterations for 64‑bit values */
        p <<= 1;

    /* Recursive step: a(n) = p + a(p - n) */
    return (seq_t)p + a_internal(p - n);
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    return a_internal(n);
}
