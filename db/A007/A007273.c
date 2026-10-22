/*
 * A007273  Inverse of 1155th cyclotomic polynomial.
 * https://oeis.org/A007273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   1/Phi_1155(x) = Prod_{d|1155} (1 - x^d)^(-mu(d)); a(n) = [x^(n
 *            mod 1155)] Prod_{d|1155, d<1155} (1 - x^d)^(-mu(d)) (a
 *            polynomial of degree 675), since the factor for d=1155 is 1/(1 -
 *            x^1155)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PERIOD 1155

seq_t a(idx_t n) {
    static long long c[PERIOD];
    static bool init = false;

    if (!init) {
        /* divisors of 1155 = 3*5*7*11 (excluding 1155), with mu(d) */
        static const int divs[15] = {1, 3, 5, 7, 11, 15, 21, 33, 35, 55, 77,
                                     105, 165, 231, 385};
        static const int mus[15] = {1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1,
                                    -1, -1, -1, -1};
        int i, j;
        memset(c, 0, sizeof(c));
        c[0] = 1;
        for (j = 0; j < 15; j++) {
            int d = divs[j];
            if (mus[j] == 1) {
                /* multiply by 1/(1 - x^d) */
                for (i = d; i < PERIOD; i++) c[i] += c[i - d];
            } else {
                /* multiply by (1 - x^d) */
                for (i = PERIOD - 1; i >= d; i--) c[i] -= c[i - d];
            }
        }
        init = true;
    }

    return (seq_t)c[n % PERIOD];
}
