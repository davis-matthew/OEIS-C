/*
 * A089463  Triangle, read by rows, of coefficients for the third iteration of
 *          the hyperbinomial transform.
 * https://oeis.org/A089463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the lower triangular matrix T(n,k)
 *            row by row, where T(n,k) = 3 * (n-k+3)^(n-k-1) * C(n,k) for k <
 *            n, T(n,n) = 1 with C(n,k) the binomial coefficient. Here a(0)=1,
 *            a(1)=3, a(2)=1, a(3)=15, a(4)=6, a(5)=1, ... as given in the
 *            OEIS entry A089463
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r and column c such that a(n) = T(r,c). */
    idx_t r = 0;
    idx_t terms = 0;
    while (terms + (r + 1) <= n) {
        terms += r + 1;
        r++;
    }
    idx_t c = n - terms;          /* 0‑based column in row r */

    if (c == r) {                 /* last element in the row */
        return (seq_t)1;
    }

    /* Compute T(r,c) = 3 * (r-c+3)^(r-c-1) * C(r,c) */
    seq_t bin   = binomial(r, c);
    seq_t base  = (seq_t)(r - c + 3);
    seq_t exp   = (seq_t)(r - c - 1);        /* >= 0 because c < r */
    seq_t pow_  = big_pow(base, exp);
    seq_t result = (seq_t)3 * pow_;
    result *= bin;
    return result;
}
