/*
 * A049627  Array T read by diagonals; T(i,j)=(i+1)*(j+1)-H(i,j), where H is
 *          the array in A049615; thus T(i,j) is the number of lattice points
 *          in rectangle having diagonal (0,0)-to-(i,j) that are visible from
 *          (i,j).
 * https://oeis.org/A049627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + [i > 0] + [j > 0] + Σ_{k=1}^{min(i,j)} μ(k) · ⌊i/k⌋ ·
 *            ⌊j/k⌋, where (i,j) are obtained from n by find d s.t. d·(d+1)/2
 *            ≤ n < (d+1)(d+2)/2, i = n − d·(d+1)/2, j = d − i. μ(k) is the
 *            Möbius function. The sum counts pairs (dx,dy) with 1 ≤ dx ≤ i, 1
 *            ≤ dy ≤ j, gcd(dx,dy)=1; the leading terms...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute μ(k) by trial division.  k is small (≤ about 150 for n≤1e4). */
static int mobius(idx_t k)
{
    if (k == 1) return 1;
    int cnt = 0;
    idx_t m = k;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int c = 0;
            while (m % p == 0) {
                m /= p;
                ++c;
                if (c > 1) return 0;  /* square factor -> μ=0 */
            }
            ++cnt;
        }
    }
    if (m > 1) ++cnt;          /* remaining prime factor */
    return (cnt % 2 == 0) ? 1 : -1;
}

/* Compute T(i,j) = number of visible lattice points in rectangle (0..i,0..j)
   from the point (i,j). */
static seq_t compute_T(idx_t i, idx_t j)
{
    long long sum = 1;                 /* the point (i,j) itself */
    if (i > 0) ++sum;
    if (j > 0) ++sum;

    idx_t limit = (i < j) ? i : j;
    for (idx_t k = 1; k <= limit; ++k) {
        int mu = mobius(k);
        if (mu == 0) continue;
        long long t = (long long)(i / k) * (long long)(j / k);
        sum += mu * t;
    }
    return (seq_t)sum;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 <= n < (d+1)(d+2)/2. */
    idx_t low = 0, high = 1;
    while (high * (high + 1) / 2 <= n) high <<= 1;

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * (mid + 1) / 2 <= n) low = mid;
        else high = mid - 1;
    }
    idx_t d = low;
    idx_t D = d * (d + 1) / 2;
    idx_t i = n - D;
    idx_t j = d - i;

    return compute_T(i, j);
}
