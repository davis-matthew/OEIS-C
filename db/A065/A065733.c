/*
 * A065733  Largest square <= n^3.
 * https://oeis.org/A065733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ( floor_sqrt(n^3) )^2 * (Largest square <= n^3)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t first to avoid intermediate 32‑bit overflows */
    seq_t tn = (seq_t)n;
    tn *= tn;          /* tn = n^2 */
    tn *= (seq_t)n;    /* tn = n^3 */

    seq_t root = floor_sqrt(tn);   /* floor( sqrt(n^3) ) */
    return root * root;            /* square it back */
}
