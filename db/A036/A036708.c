/*
 * A036708  a(n)=number of Gaussian integers z=a+bi satisfying
 *          n-1/2<|z|<=n+1/2, b>=0.
 * https://oeis.org/A036708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Gaussian integers z = a+bi with n-1/2 < |z| <=
 *            n+1/2 and b >= 0
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root and ceil-square-root for arbitrary seq_t sizes.
   No floating point and no external dependencies. */
static seq_t floor_sqrt_seq(seq_t x)
{
    if (x <= 1) return x;
    const unsigned bits = sizeof(seq_t) * 8;
    /* high <= 2^(bits/2) - 1, fits safely in seq_t. */
    seq_t low = 0;
    seq_t high = (((seq_t)1) << (bits / 2)) - 1;
    while (low < high) {
        seq_t mid = low + ((high - low + 1) >> 1);
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static seq_t ceil_sqrt_seq(seq_t v)
{
    seq_t s = floor_sqrt_seq(v);
    if (s * s < v) ++s;
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;           /* special case for n = 0 */

    const seq_t nseq = (seq_t)n;

    /* Bounds on a^2 + b^2:  n^2 - n + 1 <= a^2 + b^2 <= n^2 + n. */
    const seq_t L = nseq * nseq - nseq + (seq_t)1;
    const seq_t U = nseq * nseq + nseq;

    /* Maximum possible b: floor(sqrt(U)). */
    const seq_t Bmax = floor_sqrt_seq(U);

    seq_t cnt = 0;
    for (idx_t b = 0; b <= (idx_t)Bmax; ++b) {
        const seq_t bsq = (seq_t)b * (seq_t)b;

        /* If bsq > U, no solutions for this b (should not happen). */
        if (bsq > U) break;

        const seq_t high = U - bsq;

        /* Lower bound for a^2: if L <= bsq then 0, else L - bsq. */
        const seq_t low_sq = (L <= bsq) ? (seq_t)0 : (L - bsq);

        const seq_t a_max = floor_sqrt_seq(high);

        /* Determine minimal a. */
        const seq_t a_min = (low_sq == 0) ? (seq_t)0 : ceil_sqrt_seq(low_sq);

        if (a_max < a_min) continue;  /* no integer a in range */

        if (a_min == 0) {
            /* a = 0 counted once, positive and negative a counted twice. */
            cnt += (seq_t)(2 * a_max + 1);
        } else {
            cnt += (seq_t)(2 * (a_max - a_min + 1));
        }
    }
    return cnt;
}
