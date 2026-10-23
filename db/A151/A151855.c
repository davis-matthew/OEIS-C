/*
 * A151855  a(0)=1; a(1)=0; a(2)=1; for n>=3 if n=2^i + j with 0<=j<2^i then
 *          a(n)=a(j) + 2*a(j + 1) except we add 1 if j=2^i-1.
 * https://oeis.org/A151855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1)=0; a(2)=1; * For n≥3 let i=floor(log₂(n)),
 *            2ⁱ≤n<2ⁱ⁺¹, j=n−2ⁱ(0≤j<2ⁱ); * a(n)=a(j)+2·a(j+1) + [j==2ⁱ−1 ? 1 :
 *            0]
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* floor_log2_idx(v) returns floor(log₂(v)) for v≥1.
 * It works for 64‑bit indices and is compatible with both C and C++.
 */
static unsigned floor_log2_idx(idx_t v)
{
    unsigned r = 0;
    while (v >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Decompose n into 2ⁱ + j. */
    unsigned i = floor_log2_idx(n);
    idx_t two_pow = ((idx_t)1) << i;   /* 2ⁱ */
    idx_t j = n - two_pow;             /* 0 ≤ j < 2ⁱ */

    seq_t val_j  = a(j);
    seq_t val_j1 = a(j + 1);

    seq_t result = val_j + ((seq_t)2) * val_j1;
    if (j == two_pow - (idx_t)1)
        result += (seq_t)1;

    return result;
}
