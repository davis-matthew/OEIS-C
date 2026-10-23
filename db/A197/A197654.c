/*
 * A197654  Triangle by rows T(n,k), showing the number of meanders with
 *          length 5(n+1) and containing 5(k+1) L's and 5(n-k) R's, where L's
 *          and R's denote arcs of equal length and a central angle of 72
 *          degrees which are positively or negatively oriented.
 * https://oeis.org/A197654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) is the (n‑th) term of OEIS A197654, obtained by extracting
 *            the triangle T(n,k) of meanders of length 5(n+1). The triangle
 *            values are computed from binomial coefficients as follows: Let A
 *            = binom(r, c), B = binom(r, c+1). Then T(5, r, c) = A^5 + A^4·B
 *            + A^3·B^2 + A^2·B^3 + A·B^4 (T(5, r, r) =...
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 38, u64 n <= 125, u128 n <= 418,
 *            big any n
 * Helpers:   len_of_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/len_of_row.h"

static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (n - k + i);   /* (n−k+i) fits in idx_t */
        r = r / i;             /* exact division */
    }
    return r;
}

static seq_t compute_T5(idx_t n, idx_t k)
{
    /* T(5, n, k) for n ≥ 0, 0 ≤ k ≤ n */
    if (k == n) return 1;          /* edge case */

    seq_t A = binom_calc(n, k);
    seq_t B = binom_calc(n, k + 1);

    seq_t A2 = A * A;
    seq_t A3 = A2 * A;
    seq_t A4 = A3 * A;
    seq_t A5 = A4 * A;

    seq_t B2 = B * B;
    seq_t B3 = B2 * B;
    seq_t B4 = B3 * B;

    seq_t res = A5;
    res += A4 * B;
    res += A3 * B2;
    res += A2 * B3;
    res += A * B4;
    return res;
}

seq_t a(idx_t n)
{
    /* special first two terms */
    if (n == 0) return 1;
    if (n == 1) return 5;

    /* find the row r and offset within that row */
    idx_t r = 0;
    idx_t pos = 0;
    while (true) {
        idx_t len = len_of_row(r);
        if (pos + len > n) break;
        pos += len;
        r++;
    }
    idx_t offset = n - pos;
    idx_t k;

    if (r == 2) {
        /* row 2 is enumerated as: k=2,0,1,2 */
        if (offset == 0 || offset == 3) k = 2;
        else if (offset == 1)            k = 0;
        else                             k = 1;          /* offset==2 */
    } else {
        /* rows r=0,1,3,4,… are enumerated k=0,…,r */
        k = offset;
    }

    return compute_T5(r, k);
}
