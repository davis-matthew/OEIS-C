/*
 * A194290  Triangular array: g(n,k)=number of fractional parts (i*sqrt(3)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=2n, 1<=k<=n.
 * https://oeis.org/A194290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = g(r,k) where r and k are obtained by inverting the
 *            triangular numbering: r = ceil((sqrt(8n+1)-1)/2), k = n -
 *            r*(r-1)/2. g(r,k) = number of i (1 ≤ i ≤ 2r) such that the
 *            fractional part of i*sqrt(3) lies in [(k-1)/r, k/r)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

const uint64_t SQRT3_NUM = 1732050807568ULL;    /* sqrt(3) ≈ 1.7320508075688773 */
const uint64_t SQRT3_DEN = 1000000000000ULL;    /* denominator 1e12 */

/* Count i in [1,2r] whose fractional part of i*sqrt(3) lies in the
   interval [(k-1)/r, k/r).  Use integer arithmetic only. */
static seq_t count_interval(idx_t r, idx_t k)
{
    idx_t count = 0;
    uint64_t r64 = (uint64_t)r;
    uint64_t k1 = (uint64_t)(k - 1);
    uint64_t k2 = (uint64_t)k;

    for (idx_t i = 1; i <= 2*r; ++i) {
        __int128 prod = (__int128)i * (__int128)SQRT3_NUM;  /* i * sqrt3_num */
        uint64_t rem = (uint64_t)(prod % SQRT3_DEN);        /* remainder mod denominator */
        uint64_t lhs = rem * r64;                           /* rem * r */
        if (lhs >= k1 * SQRT3_DEN && lhs < k2 * SQRT3_DEN)
            ++count;
    }
    return (seq_t)count;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n.  r will be small (<200 for n<10000). */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        ++r;
    }

    /* Column k in that row */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;

    return count_interval(r, k);
}
