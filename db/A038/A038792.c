/*
 * A038792  Rectangular array defined by T(i,1) = T(1,j) = 1 for i >= 1 and j
 *          >= 1; T(i,j) = max(T(i-1,j) + T(i-1,j-1), T(i-1,j-1) + T(i,j-1))
 *          for i >= 2, j >= 2, read by antidiagonals.
 * https://oeis.org/A038792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,j), where (i,j) is the n‑th term in the *
 *            anti‑diagonal enumeration of the array defined by *
 *            T(i,1)=T(1,j)=1 * T(i,j)=F(i+j-1, min(i-1,j-1)) * with *
 *            F(n,k)=∑_{s=0..k} C(n‑1‑s, s) (C = binomial coefficient). * *
 *            This implementation computes the position (i,j) from n and then
 *            * evaluat...
 * Range:     u8 n <= 94, u16 n <= 307, u32 n <= 1049, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Integer square‑root (floor).  Works for idx_t values that fit in 64 bits. */
static inline idx_t isqrt_u64(idx_t x)
{
    double d = sqrt((double)x);
    idx_t r = (idx_t)d;
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* Compute binomial coefficient C(n,k) as a seq_t.  Uses a multiplicative
 * approach that keeps intermediate values integral.  Works for n ≤ 2^64-1
 * and k ≤ n, and for seq_t types that support the usual arithmetic
 * operators. */
static inline seq_t binom_u64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / ((seq_t)i);
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n = 0, but keep safe */

    /* Find the diagonal sum d = i + j that contains the n‑th element.
     * Solve (d-1)*d/2 >= n.  We use the quadratic formula and an integer
     * sqrt to avoid overflow. */
    idx_t d;
    {
        idx_t val = n * 8 + 1;           /* 8n+1 fits in 64 bits for the
                                           typical limits of this sequence */
        idx_t sqrtv = isqrt_u64(val);
        d = (1 + sqrtv) >> 1;           /* floor of (1+sqrt)/2 */
        if ((d * (d - 1)) / 2 < n) ++d; /* ensure ceil */
    }

    /* Compute the position within the diagonal. */
    idx_t prev = ((d - 1) * (d - 2)) / 2;   /* elements before diagonal d */
    idx_t pos  = n - prev;                 /* 1‑based position inside diagonal */

    /* Coordinates (i,j) – note i grows, j decreases along a diagonal. */
    idx_t i = pos;
    idx_t j = d - pos;

    /* Sum the binomials: T(i,j) = ∑_{s=0..min(i-1,j-1)} C(i+j-2-s, s) */
    idx_t limit = (i < j ? i : j) - 1;      /* min(i,j)-1 = min(i-1,j-1) */
    seq_t sum = 0;
    for (idx_t s = 0; s <= limit; ++s) {
        sum = sum + binom_u64(i + j - 2 - s, s);
    }
    return sum;
}
