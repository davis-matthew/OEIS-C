/*
 * A176777  a(n) = 2^n - n*(n-3).
 * https://oeis.org/A176777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - n*(n-3)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the few small cases where n-3 would underflow an unsigned type */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)6;

    seq_t pown = pow2(n);          /* 2^n  */
    seq_t sub  = (seq_t)n * ((seq_t)n - 3);  /* n*(n-3) for n ≥ 3 (non‑negative) */
    return pown - sub;             /* a(n) = 2^n - n*(n-3) */
}
