/*
 * A120465  a(0)=1, a(1)=36, a(n)=204*a(n-2).
 * https://oeis.org/A120465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n%2==0) ? 204^(n/2) : 36 * 204^((n-1)/2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

/* Computes sequence A120465: a(0)=1, a(1)=36, a(n)=204*a(n-2) for n>=2. */
seq_t a(idx_t n)
{
    idx_t k = n >> 1;                     /* floor(n/2)   */
    seq_t val = seq_pow((seq_t)204, k);   /* 204^k        */

    if (n & 1U)                            /* n odd ?      */
        val = val * (seq_t)36;             /* multiply by 36 */

    return val;
}
