/*
 * A136715  Triangle T(n,k), 1 <= k <= n, read by rows: T(n,k) is the number
 *          of permutations of the set {2,4,6,...,2n} with k excedances.
 *          Equivalently, T(n,k) is the number of permutations in the
 *          symmetric group S_n having k multiplicative 2-excedances.
 * https://oeis.org/A136715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the value T(r,k) of the triangle A136715. Let r be the
 *            smallest integer with r(r+1)/2 ≥ n. Let k = n - r(r-1)/2 (1 ≤ k
 *            ≤ r). Then T(r,k) = (⌊r/2⌋)! * (⌊(r+1)/2⌋)! * C(⌊r/2⌋,
 *            k-⌊(r+1)/2⌋) * C(⌊(r+1)/2⌋, k-⌊r/2⌋) (Here C(n,k)=0 if k<0 or
 *            k>n.)
 * Range:     u8 n <= 18, u16 n <= 41, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

static inline seq_t mul_seq(seq_t a, seq_t b)
{
    /* Use 128‑bit intermediate to avoid overflow when possible. */
    return (seq_t)(((__int128)a) * b);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* optional, outside OEIS offset */

    /* Find r such that r*(r+1)/2 ≥ n.  r ≈ ceil((sqrt(8n+1)-1)/2) */
    long double d = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);          /* floor */
    if ( (r * (r + 1)) / 2 < n ) r++;              /* ensure r is large enough */

    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;       /* 1‑based position inside the row */

    /* Compute T(r,k) via the explicit formula. */
    idx_t a1 = r / 2;            /* ⌊r/2⌋ */
    idx_t a2 = (r + 1) / 2;      /* ⌊(r+1)/2⌋ */

    /* Bounds check for binomial arguments.   */
    if (k < a1 || k < a2) return 0;

    idx_t y1 = k - a2;           /* k-⌊(r+1)/2⌋ */
    idx_t y2 = k - a1;           /* k-⌊r/2⌋ */

    if (y1 > a1 || y2 > a2) return 0;

    seq_t fac1 = factorial(a1);
    seq_t fac2 = factorial(a2);
    seq_t bin1 = binomial(a1, y1);
    seq_t bin2 = binomial(a2, y2);

    seq_t res = mul_seq(fac1, fac2);
    res = mul_seq(res, bin1);
    res = mul_seq(res, bin2);

    return res;
}
