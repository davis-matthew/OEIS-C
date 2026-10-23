/*
 * A194102  a(n) = Sum_{j=1..n} floor(j*sqrt(2)); n-th partial sum of Beatty
 *          sequence for sqrt(2), A001951.
 * https://oeis.org/A194102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} floor(j*sqrt(2)) (partial sum of the Beatty
 *            sequence for sqrt(2))
 * Range:     u8 n <= 18, u16 n <= 304, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Constants for integer arithmetic approximation of sqrt(2) */
#define NUM_SQRT2          1414213562373095048ULL   /* floor(sqrt(2)*1e18) */
#define DEN                1000000000000000000ULL   /* 1e18 */

/* Compute a(n).  The function works for any seq_t width and is O(n). */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j)
    {
        /* floor(j * sqrt(2)) using 128‑bit intermediate to avoid overflow */
        seq_t term = (seq_t)((unsigned __int128)j * NUM_SQRT2 / DEN);
        sum += term;
    }
    return sum;
}
