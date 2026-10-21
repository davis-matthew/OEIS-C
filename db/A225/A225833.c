/*
 * A225833  Number of binary pattern classes in the (9,n)-rectangular grid:
 *          two patterns are in the same class if one of them can be obtained
 *          by a reflection or 180-degree rotation of the other.
 * https://oeis.org/A225833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 512*a(n‑1)+512*a(n‑2) – 262144*a(n‑3) – 2^(5n‑3)*15 with
 *            a(0)=1, a(1)=272, a(2)=66048. This recurrence follows the OEIS
 *            definition for sequence A225833
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)272;
    if (n == 2) return (seq_t)66048;

    /* previous three values */
    seq_t a0 = (seq_t)1;          /* a(k-3) */
    seq_t a1 = (seq_t)272;        /* a(k-2) */
    seq_t a2 = (seq_t)66048;      /* a(k-1) */

    const seq_t p9  = pow2((idx_t)9);   /* 512   */
    const seq_t p18 = pow2((idx_t)18);  /* 262144 */
    const seq_t constant_fifteen = (seq_t)15;

    for (idx_t k = 3; k <= n; ++k) {
        seq_t term1 = p9  * a2;                /* 512 * a(k-1) */
        seq_t term2 = p9  * a1;                /* 512 * a(k-2) */
        seq_t term3 = p18 * a0;                /* 262144 * a(k-3) */
        seq_t exp  = pow2((idx_t)(5 * k - 3)); /* 2^(5k-3) */
        seq_t term4 = exp * constant_fifteen;  /* 2^(5k-3) * 15 */

        seq_t ak = term1 + term2 - term3 - term4;

        a0 = a1;
        a1 = a2;
        a2 = ak;
    }
    return a2;   /* a(n) */
}
