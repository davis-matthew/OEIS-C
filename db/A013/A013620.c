/*
 * A013620  Triangle of coefficients in expansion of (2+3*x)^n.
 * https://oeis.org/A013620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = coefficient T(r,k) in the expansion of (2+3*x)^r, where r
 *            is the unique integer with r(r+1)/2 <= n < (r+1)(r+2)/2, and k =
 *            n - r(r+1)/2. Then a(n) = C(r,k) * 2^(r-k) * 3^k
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 127, u64 n <= 450,
 *            u128 n <= 1682, big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* find the row r that contains the index n.
       The total number of terms before row r is r(r+1)/2. */
    idx_t r   = 0;
    idx_t cum = 0;          /* sum of lengths of rows before r   */
    while (cum + r + 1 <= n) {
        cum += r + 1;       /* add length of current row r      */
        ++r;
    }

    /* position k within row r (0‑based) */
    idx_t k = n - cum;

    /* coefficient T(r,k) = C(r,k) * 2^(r-k) * 3^k */
    seq_t res = binomial(r, k);
    res *= pow2(r - k);
    res *= pow3(k);
    return res;
}
