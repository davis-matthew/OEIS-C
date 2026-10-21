/*
 * A085215  Square array A(x,y) = the number whose factorial expansion A007623
 *          is that of x and y concatenated; zero expanded as empty string;
 *          read by ascending antidiagonals: A(0,0), A(1,0), A(0,1), A(2,0),
 *          A(1,1), A(0,2), ...
 * https://oeis.org/A085215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(x,y) where A(x,y) is the number whose factorial *
 *            expansion is the concatenation of the factorial expansions of x
 *            and y. * (x,y) are the antidiagonal coordinates of n obtained
 *            with * antidiagonal_coords(n,&i,&j); the function returns them
 *            as * (i=y, j=x) – i is the y‑coordinate, j is the...
 * Range:     u8 n <= 71, u16 n <= 2651, u32 n <= 9020, u64 n <= 9020,
 *            u128 n <= 9020, big any n
 * Helpers:   antidiagonal_coords, factorial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/antidiagonal_coords.h"
#include "../utils/factorial.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);   /* i = y, j = x */

    /* factorial digits of x (least significant first) */
    unsigned int digits_x[32];
    unsigned int len_x = 0;
    idx_t tmp = j;
    unsigned int pos = 1;
    while (tmp > 0) {
        digits_x[len_x++] = (unsigned int)(tmp % (pos + 1));
        tmp /= (pos + 1);
        ++pos;
    }

    /* factorial digits of y (least significant first) */
    unsigned int digits_y[32];
    unsigned int len_y = 0;
    tmp = i;
    pos = 1;
    while (tmp > 0) {
        digits_y[len_y++] = (unsigned int)(tmp % (pos + 1));
        tmp /= (pos + 1);
        ++pos;
    }

    /* Result starts with the value of y.  y occupies the low factorial places. */
    seq_t result = (seq_t)i;

    /* factorial corresponding to the first position above y.  */
    seq_t fact = factorial((idx_t)len_y + 1);

    /* Add digits of x shifted to higher factorial positions. */
    for (unsigned int k = 0; k < len_x; ++k) {
        unsigned int d = digits_x[k];
        if (d)
            result += mul_small(fact, d);
        fact = mul_small(fact, (idx_t)len_y + k + 2);
    }

    return result;
}
