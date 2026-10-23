/*
 * A125585  Array of constant-spaced integers read by antidiagonals.
 * https://oeis.org/A125585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   a(n) is obtained by reading, in antidiagonal order, a table
 *            whose rows are arithmetic progressions. For row i the common
 *            difference is k = min { m | m(m+1)/2 ≥ i }, and the first term
 *            is t = i – k(k−1)/2. The entry at column j is then a_{i,j} = t +
 *            (j−1)k. The n‑th term is the (i,j) with i+j = s,...
 * Range:     u8 n <= 1748, u16 n <= 8217, u32 n <= 8217, u64 n <= 8217,
 *            u128 n <= 8217, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* triangular number in 128‑bit to avoid overflow */
static inline __uint128_t triangular128(idx_t m)
{
    return (__uint128_t)m * (m + 1) / 2;
}

/* find minimal k such that k*(k+1)/2 ≥ val   (val >= 1) */
static idx_t find_min_k_ge(idx_t val)
{
    idx_t lo = 1, hi = val;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (triangular128(mid) >= val)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Step 1: locate the antidiagonal s for n.            */
    idx_t s_minus_1 = find_min_k_ge(n);   /* t in earlier explanation */
    idx_t s = s_minus_1 + 1;              /* actual sum index */

    /* previous cumulative terms before this antidiagonal */
    idx_t prev_terms = (idx_t)triangular128(s_minus_1 - 1);   /* T_{s-2} */

    idx_t r = n - prev_terms;           /* rank within this antidiagonal */
    idx_t i = s - r;                     /* row index  (1‑based) */
    idx_t j = r;                         /* column index (1‑based) */

    /* Step 2: determine row parameters k and t_row for row i. */
    idx_t k = find_min_k_ge(i);          /* common difference of row i */
    idx_t prev_k = (idx_t)triangular128(k - 1);   /* T_{k-1} */
    idx_t t_row = i - prev_k;            /* first term index (1‑based) */

    /* Step 3: compute the actual value. */
    seq_t result = (seq_t)t_row + (seq_t)(j - 1) * (seq_t)k;
    return result;
}
