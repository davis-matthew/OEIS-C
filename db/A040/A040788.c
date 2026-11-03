/*
 * A040788  Continued fraction for sqrt(817).
 * https://oeis.org/A040788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = floor(sqrt(817)) = 28. For n ≥ 1, a(n) repeats the period
 *            [1, 1, 2, 1, 1, 56]
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(817)) using only integer arithmetic. */
static seq_t _sqrt817(void)
{
    idx_t i = 0;
    /* The square of 28 is 784 and of 29 is 841, so the loop will
       terminate after a few iterations. */
    while ((i + 1) * (i + 1) <= 817u)
        ++i;
    return (seq_t)i;
}

/* Periodic part of the continued fraction for sqrt(817). */
static const seq_t _cf_period[6] = { (seq_t)1, (seq_t)1, (seq_t)2,
                                     (seq_t)1, (seq_t)1, (seq_t)56 };

seq_t a(idx_t n)
{
    if (n == 0)
        return _sqrt817();

    /* n – 1 is always non‑negative for n ≥ 1, and the % operator works
       correctly on unsigned idx_t. */
    return _cf_period[(n - 1) % 6];
}
