/*
 * A180867  Square array read by antidiagonals: T(m,n) is the Wiener index of
 *          the Dutch windmill graph D(m,n) (m>=3, n>=1).
 * https://oeis.org/A180867
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Wiener index of the Dutch windmill graph D(m,n) * read by
 *            antidiagonals, where (m,n) are computed from the index n. * For
 *            m odd: T(m,n) = (1/8) * n * (m^2 - 1) * (2mn - m - 2n + 2) * For
 *            m even: T(m,n) = (1/8) * n * m^2 * (2mn - m - 2n + 2)
 * Range:     u8 n <= 5, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(x)) */
static seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = x + 1;
    while (lo < hi) {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* undefined for n<3, return 0 */

    /* k is the zero‑based position in the antidiagonal sequence
       (a(3) corresponds to k=1). */
    idx_t k = n - 2;              /* k >= 1 */

    /* Solve for x where x(x+1)/2 >= k */
    seq_t target = (seq_t)1 + (seq_t)8 * (seq_t)k;          /* 1+8k */
    seq_t r = floor_sqrt(target);                          /* floor sqrt(1+8k) */
    seq_t x = (r - (seq_t)1) >> 1;                          /* (r-1)/2 */
    /* Ensure x satisfies the inequality */
    while ((x * (x + (seq_t)1) / (seq_t)2) < (seq_t)k) x++;

    seq_t s = x + (seq_t)3;        /* sum m+n of current antidiagonal */

    /* cumulative terms before this antidiagonal */
    seq_t prev = ((s - (seq_t)4) * (s - (seq_t)3)) / (seq_t)2;
    seq_t pos = (seq_t)k - prev;   /* 1-based position within the antidiagonal */

    seq_t mseq = s - pos;          /* m = s - pos */
    seq_t nseq = pos;              /* n = pos */

    seq_t m2 = mseq * mseq;
    seq_t tmp = (seq_t)2 * mseq * nseq
                - mseq
                - (seq_t)2 * nseq
                + (seq_t)2;

    seq_t factor;
    if ((mseq & (seq_t)1) == 0) {  /* m is even */
        factor = nseq * m2 * tmp;
    } else {                       /* m is odd */
        factor = nseq * (m2 - (seq_t)1) * tmp;
    }

    return factor / (seq_t)8;
}
