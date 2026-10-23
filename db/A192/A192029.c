/*
 * A192029  Square array read by antidiagonals: W(n,m) (n >= 3, m >= 1) is the
 *          Wiener index of the graph G(n,m) obtained from an n-wheel graph by
 *          adjoining at each of its n rim nodes a path with m nodes (if m=1,
 *          then the n-wheel is not modified).
 * https://oeis.org/A192029
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the infinite array of W(n,m) =
 *            (1/6)*n*m*(3*n*m + 3*n*m² + 2 - 6*m - 2*m²) (with n≥3, m≥1)
 *            along its antidiagonals, starting from n=3, m=1. The index n of
 *            the sequence is offset 3 (a(3) = W(3,1))
 * Range:     u8 n <= 14, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a 64‑bit unsigned integer (floor of sqrt).  */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFull;      /* sqrt(2^64‑1) < 2^32 */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;             /* fits in 64 bits for mid < 2^32 */
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi; /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;              /* below offset – any value is allowed */

    /* Zero–based index inside the infinite array.  */
    idx_t k = n - 3;

    /* Find the antidiagonal (sum s = n+m) that contains k.
       Number of pairs with sum ≤ s is (s-3)(s-2)/2.  */
    idx_t discr   = 1 + 8 * k;
    idx_t sqrt_d  = isqrt_u64(discr);
    idx_t t       = (sqrt_d - 1) / 2;    /* floor((-1+sqrt)/2)   */
    idx_t s       = t + 3;               /* candidate sum        */

    /* Adjust s if it is not large enough.  */
    idx_t tri_s = ((s - 3) * (s - 2)) / 2;
    if (tri_s <= k) ++s;

    /* Position within the found antidiagonal.  */
    idx_t tri_prev = ((s - 4) * (s - 3)) / 2;
    idx_t pos = k - tri_prev;              /* 0‑based  */

    /* Corresponding pair (n',m').  */
    seq_t n_val = (seq_t)(s - 1 - pos);    /* n decreases  */
    seq_t m_val = (seq_t)(pos + 1);        /* m increases  */

    /* Compute the Wiener index using the closed formula.  */
    seq_t nseq = n_val;
    seq_t mseq = m_val;
    seq_t nm   = nseq * mseq;
    seq_t nmsq = nm * mseq;                /* n*m² */

    seq_t innerm = (seq_t)3 * nm
                 + (seq_t)3 * nmsq
                 + (seq_t)2
                 - (seq_t)6 * mseq
                 - (seq_t)2 * mseq * mseq;

    seq_t result = nm * innerm / (seq_t)6;
    return result;
}
