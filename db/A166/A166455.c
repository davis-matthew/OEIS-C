/*
 * A166455  Triangle read by rows, twice Pascal's triangle minus Sierpinski's
 *          gasket: 2*A007318 - A047999.
 * https://oeis.org/A166455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*binom(r,k) – (binom(r,k) mod 2) where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n – r*(r+1)/2
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binom_n_k.h"

seq_t a(idx_t n) {
    /* find the row r that contains n */
    long double ld = sqrtl((long double)8 * n + 1.0L);
    idx_t r = (idx_t)((ld - 1.0L) / 2.0L);

    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    while (tri > n) {
        r--;
        tri = (unsigned __int128)r * (r + 1) / 2;
    }
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }

    idx_t k = n - (idx_t)tri;

    seq_t c = binom_n_k(r, k);
    return (c << 1) - (c & 1);
}
