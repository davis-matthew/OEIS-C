/*
 * A038227  Triangle whose (i,j)-th entry is binomial(i,j)*3^(i-j)*9^j.
 * https://oeis.org/A038227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 3^(i-j) * 9^j * where i is the row such
 *            that n < (i+1)(i+2)/2 * and j = n - i(i+1)/2
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* Find the row i containing the term a(n) */
    idx_t i = 0;
    /* cumulative terms up to and including row i:
       cum_i = (i+1)(i+2)/2
       we stop when n < cum_i
    */
    while (1) {
        idx_t cum = (i + 1) * (i + 2) / 2;
        if (n < cum)
            break;
        ++i;
    }

    /* Position j within row i */
    idx_t prev = i * (i + 1) / 2;  /* terms before row i */
    idx_t j = n - prev;            /* 0‑based column index */

    /* Compute the entry: binomial(i,j) * 3^(i-j) * 9^j
       Using 9^j = 3^(2j)
    */
    seq_t val = binomial(i, j);
    val *= pow3(i - j);   /* multiply by 3^(i-j) */
    val *= pow3(2 * j);   /* multiply by 3^(2j) = 9^j */

    return val;
}
