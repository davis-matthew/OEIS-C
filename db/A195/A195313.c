/*
 * A195313  Generalized 13-gonal numbers: m*(11*m-9)/2 with m = 0, 1, -1, 2,
 *          -2, 3, -3, ...
 * https://oeis.org/A195313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m*(11*m-9)/2 with m = 0, 1, -1, 2, -2, 3, -3, …
 * Range:     u8 n <= 13, u16 n <= 217, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }

    /* Determine |m| and its sign.
       For n >= 1:
          if n is odd   -> m = +(n+1)/2
          if n is even  -> m = -(n/2)          */
    idx_t abs_m = (n >> 1) + (n & 1);   /* (n+1)/2 for odd, n/2 for even */
    bool neg = !(n & 1);                /* true iff n is even (negative m) */

    __int128 val;
    if (neg) {
        /* m = -abs_m, 11*m - 9 = -11*abs_m - 9,
           product = abs_m*(11*abs_m + 9) */
        val = (__int128)abs_m * ((__int128)11 * abs_m + 9);
    } else {
        /* m = +abs_m,   11*m - 9 = 11*abs_m - 9 */
        val = (__int128)abs_m * ((__int128)11 * abs_m - 9);
    }
    val /= 2;

    return (seq_t)val;
}
