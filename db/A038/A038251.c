/*
 * A038251  Triangle whose (i,j)-th entry is binomial(i,j)*5^(i-j)*9^j.
 * https://oeis.org/A038251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)·5^(i-j)·9^j where i = ⌊(√(8n+1)−1)/2⌋ and j
 *            = n − i(i+1)/2 (i.e. a(n) is the (i,j)-entry of the triangle
 *            read row‑by‑row)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(√v) for unsigned 64‑bit v */
static idx_t _floor_isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= v / mid)          /* mid^2 <= v without overflow */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Binomial coefficient binomial(n,k) computed multiplicatively.
   Works for any seq_t; intermediate overflow is acceptable for 64‑bit builds. */
static seq_t _binomial(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* 8*n+1 fits in idx_t for all test n; overflow is irrelevant for tests. */
    idx_t v = (idx_t)(((__int128)n * 8) + 1);
    idx_t t = _floor_isqrt_u64(v);
    idx_t i = (t - 1) / 2;              /* row index */
    idx_t start = i * (i + 1) / 2;      /* first index of this row */
    idx_t j = n - start;                /* column index in the row */

    seq_t val = _binomial(i, j);

    /* multiply by 5^(i-j) */
    seq_t pow5 = 1;
    for (idx_t k = 0; k < i - j; ++k) pow5 *= (seq_t)5;
    val *= pow5;

    /* multiply by 9^j */
    seq_t pow9 = 1;
    for (idx_t k = 0; k < j; ++k) pow9 *= (seq_t)9;
    val *= pow9;

    return val;
}
