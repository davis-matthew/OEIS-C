/*
 * A038288  Triangle whose (i,j)-th entry is binomial(i,j)*8^(i-j)*10^j.
 * https://oeis.org/A038288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r, j) * 8^(r-j) * 10^j, * where r is the row
 *            containing n (0‑based) and * j = n – r*(r+1)/2
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--- include required helpers ----------------------------------------*/
#include "../utils/binomial.h"
#include "../utils/mul_small.h"

/*--- compute a(n) ----------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    __int128 cum = 1;                    /* cumulative count up to row r */
    while (cum <= n) {
        ++r;
        cum += (__int128)(r + 1);        /* add count of row r */
    }

    /* Elements before row r */
    idx_t prev = (idx_t)(cum - (__int128)(r + 1));   /* r*(r+1)/2 */
    idx_t j = n - prev;                    /* column index in row r */

    /* Compute binomial(r, j) */
    seq_t bin = binomial(r, j);

    /* Compute 8^(r-j) */
    seq_t pow8 = 1;
    for (idx_t k = 0; k < r - j; ++k) {
        pow8 = mul_small(pow8, 8);
    }

    /* Compute 10^j */
    seq_t pow10 = 1;
    for (idx_t k = 0; k < j; ++k) {
        pow10 = mul_small(pow10, 10);
    }

    /* Combine all parts */
    seq_t result = bin;
    result = result * pow8;
    result = result * pow10;

    return result;
}
