/*
 * A055589  Convolution of A049612 with A011782.
 * https://oeis.org/A055589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0}^{min(4,n‑1)} (-1)^j · C(4,j) · C(n‑1‑j+4,4) ·
 *            2^{\,n‑1‑j} (derived from the generating function
 *            x·(1‑x)^4/(1‑2x)^5)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t used in idx_t */

static seq_t pow2_idx(idx_t e)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;

    while (e)
    {
        if (e & (idx_t)1)
            result = result * base;
        base   = base * base;
        e     >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* a(0) = 0 */

    /* C(4,j) for j = 0..4 */
    const int choose[5] = {1, 4, 6, 4, 1};

    /* The largest j that contributes */
    int maxj = (n - (idx_t)1 < 4 ? (int)(n - (idx_t)1) : 4);

    seq_t pos = (seq_t)0;   /* positive contributions */
    seq_t neg = (seq_t)0;   /* negative contributions */

    for (int j = 0; j <= maxj; ++j)
    {
        idx_t m = (n - (idx_t)1) - (idx_t)j;   /* m = n‑1‑j */

        /* C(m+4,4) = (m+4)(m+3)(m+2)(m+1) / 24 */
        seq_t num = (seq_t)(m + (idx_t)4) *
                    (seq_t)(m + (idx_t)3) *
                    (seq_t)(m + (idx_t)2) *
                    (seq_t)(m + (idx_t)1);
        seq_t binom = num / (seq_t)24;

        seq_t pow = pow2_idx(m);                 /* 2^m */
        seq_t term = pow * binom;                /* binom * 2^m */
        term = term * (seq_t)choose[j];          /* multiplied by C(4,j) */

        if ((j & 1) == 0)                       /* even j → positive sign */
            pos += term;
        else                                     /* odd j  → negative sign */
            neg += term;
    }

    return pos - neg;   /* pos ≥ neg always */
}
