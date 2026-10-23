/*
 * A139154  a(n) = (n!+7)/7.
 * https://oeis.org/A139154
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! + 7) / 7 Recurrence used: a(n) = n * a(n-1) - (n-1),
 *            for n ≥ 8 Offset: a(7) = 721
 * Range:     u8 none, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 7. For smaller arguments
       the return value is unspecified; we simply return 0. */
    if (n < 7)
        return 0;

    /* Starting value a(7) */
    seq_t current = 721;  /* (7! + 7)/7 = 721 */
    /* Compute iteratively up to n using the recurrence
       a(k) = k * a(k-1) - (k-1). */
    for (idx_t k = 8; k <= n; ++k)
    {
        seq_t tmp = (seq_t)k * current;   /* k * a(k-1) */
        tmp -= (k - 1);                   /* subtract (k-1) */
        current = tmp;                    /* a(k) */
    }
    return current;
}
