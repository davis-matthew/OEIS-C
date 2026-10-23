/*
 * A038207  Triangle whose (i,j)-th entry is binomial(i,j)*2^(i-j).
 * https://oeis.org/A038207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice, changed
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 79, u32 n <= 258, u64 n <= 956,
 *            u128 n <= 3511, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A038207: flatten the upper-triangular array T(n,k)=C(n,k)*2^(n-k).
   For index n, find row r so that r(r+1)/2 ≤ n < (r+1)(r+2)/2.
   Let k = n - r(r+1)/2. Then a(n) = C(r,k) * 2^(r-k). */
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r using a small linear search (n ≤ 10000 is trivial). */
    idx_t r = 0;          /* current row */
    idx_t cum = 0;        /* cumulative count of terms up to row r-1 */
    while (1) {
        idx_t row_len = r + 1;               /* terms in this row */
        if (cum + row_len > n) break;        /* n lies in this row */
        cum += row_len;
        ++r;
    }

    /* Column index within the row */
    idx_t k = n - cum;

    /* Compute the entry: C(r,k) * 2^(r-k) */
    seq_t bin   = binomial(r, k);
    seq_t pow   = pow2(r - k);
    return bin * pow;
}
