/*
 * A124732  Triangle P*M, where P is the Pascal triangle written as an
 *          infinite lower triangular matrix and M is the infinite bidiagonal
 *          matrix with (1,2,1,2,...) in the main diagonal and (2,1,2,1,...)
 *          in the subdiagonal.
 * https://oeis.org/A124732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = binomial(n,k)*(3n-(-1)^k*(n-2k))/(2n) a(n) is the
 *            row‑by‑row reading of the triangle
 * Range:     u8 n <= 31, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static seq_t binom(idx_t n, idx_t k) {
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i);
        result = result / i;           /* division is exact at each step */
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* offset is 1, a(0) unused */

    /* Find row r: smallest r with r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cumulative = 1;               /* 1 = r(r+1)/2 for r = 1 */
    while (cumulative < n) {
        ++r;
        cumulative += r;                /* add new row length */
    }

    /* Column index k (1‑based) */
    idx_t prev_cumulative = (r - 1) * r / 2;   /* (r-1)r/2 */
    idx_t k = n - prev_cumulative;

    /* Compute binomial coefficient */
    seq_t bin = binom(r, k);

    /* Compute T(r,k) using the simplified integer form:
       odd  k:  bin * (2r - k) / r
       even k:  bin * (r + k)  / r
    */
    seq_t row_seq = (seq_t)r;
    seq_t tmp;
    if (k & 1) {                          /* k is odd */
        tmp = (seq_t)2 * row_seq - (seq_t)k;
    } else {                              /* k is even */
        tmp = row_seq + (seq_t)k;
    }

    /* Final value */
    return (bin * tmp) / row_seq;
}
