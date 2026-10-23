/*
 * A034007  First differences of A045891.
 * https://oeis.org/A034007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+5)·2^(n-4) for n ≥ 3; explicit initial values: a(0)=1,
 *            a(1)=0, a(2)=2, a(3)=4
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly to avoid a negative exponent. */
    if (n == 0) return 1;          /* a(0) */
    if (n == 1) return 0;          /* a(1) */
    if (n == 2) return 2;          /* a(2) */
    if (n == 3) return 4;          /* a(3)   (special case, avoids 2^(-1) ) */

    /* From here n ≥ 4, so the exponent is non‑negative. */
    seq_t term = pow2(n - 4);        /* 2^(n-4) */
    return ((seq_t)n + 5) * term;    /* (n+5)·2^(n-4) */
}
