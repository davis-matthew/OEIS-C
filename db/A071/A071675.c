/*
 * A071675  Array read by antidiagonals of trinomial coefficients.
 * https://oeis.org/A071675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 265, u32 n <= 884, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A071675 – Trinomial coefficients read by antidiagonals
 *
 *  a(n) = coefficient of x^j in (1+x+x^2)^i
 *  where (i,j) are obtained by Cantor's enumeration of pairs:
 *      let s     = floor((sqrt(8n+1)-1)/2)   // largest s with s(s+1)/2 ≤ n
 *      let tri   = s(s+1)/2
 *      let offset = n - tri
 *      i = offset
 *      j = s - offset
 *
 *  If j > 2*i the coefficient is zero.
 *  Otherwise a(n) = Σ_{c=max(0,j-i)}^{⌊j/2⌋} C(i,c) * C(i-c, j-2c),
 *  where C(a,b) is the binomial coefficient a choose b.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"    /* provides seq_t binomial(idx_t, idx_t) */

/* Find the largest integer s such that s*(s+1)/2 ≤ n.
 * n is an idx_t (unsigned 64‑bit).  The result fits in idx_t. */
static idx_t triangular_root(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 0xFFFFFFFFULL;                 /* 4 294 967 295 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Cantor enumeration -> (row i, column j) */
    idx_t s      = triangular_root(n);
    __uint128_t tri128 = (__uint128_t)s * (s + 1) / 2;
    idx_t tri    = (idx_t)tri128;                 /* safe: s <= 2^32-1 */
    idx_t offset = n - tri;
    idx_t i      = offset;                       /* row  */
    idx_t j      = s - offset;                    /* column */

    if (j > 2 * i)
        return (seq_t)0;

    /* compute coefficient of x^j in (1+x+x^2)^i   */
    idx_t start_c = (j > i) ? (j - i) : 0;
    idx_t end_c   = j / 2;

    if (start_c > end_c)
        return (seq_t)0;

    seq_t result = (seq_t)0;
    for (idx_t c = start_c; c <= end_c; ++c) {
        seq_t a_c = binomial(i, c);              /* C(i,c)  */
        seq_t b_c = binomial(i - c, j - 2 * c);   /* C(i-c, j-2c) */
        result += a_c * b_c;
    }
    return result;
}
