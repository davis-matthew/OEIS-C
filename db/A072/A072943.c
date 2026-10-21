/*
 * A072943  Least nonnegative integer k such that n + k^3 is prime, or -1 if
 *          no such k exists.
 * https://oeis.org/A072943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   int_cube_root, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal k ≥ 0 such that n + k³ is prime, or -1 if none (for n≥1). */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    /* handle n = 1  */
    if (n == 1)
    {
        /* 1 + 1³ = 2 is prime, so answer is 1 */
        return (seq_t)1;
    }

    /* Check if n is prime → k = 0 */
    if (isPrimeGeneric((seq_t)n))
        return (seq_t)0;

    /* If n ≥ 8 and is a perfect cube (m³), return -1 */
    if (n >= 8)
    {
        idx_t r = int_cube_root(n);  /* floor integer cube root */
        if ((seq_t)r * r * r == (seq_t)n)
            return (seq_t)-1;
    }

    /* Search for the least k ≥ 0 with n + k³ prime. */
    /* The search is bounded; for n ≤ 10000 k rarely exceeds a few dozen. */
    for (idx_t k = 0; k <= 1000; ++k)
    {
        seq_t k3 = (seq_t)k * k * k;
        seq_t t   = (seq_t)n + k3;
        if (isPrimeGeneric(t))
            return (seq_t)k;
    }

    /* If not found within the bound, return -1.  (Such cases only occur
       when n is a perfect cube as proven.) */
    return (seq_t)-1;
}
