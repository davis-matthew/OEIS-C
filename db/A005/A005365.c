/*
 * A005365  Hoggatt sequence with parameter d=7.
 * https://oeis.org/A005365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k), T(n,k) = Product_{j=0..6} C(n+j,k) /
 *            Product_{m=1..6} C(k+m,m) (the 7F6(...;-1) series term by term);
 *            T(n,0) = 1, T(n,k+1) = T(n,k) * Product_{j=0..6} (n-k+j) /
 *            Product_{m=1..7} (k+m). Fixed-width builds evaluate T(n,k) from
 *            its prime factorization (Legendre) so no int...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   a_fixed_A005365, leg_A005365, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/a_fixed_A005365.h"
#include "../utils/leg_A005365.h"

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_fixed_A005365(n);
    seq_t t = 1, sum = 1;
    for (idx_t k = 0; k < n; k++) {
        seq_t num = 1, den = 1;
        for (idx_t j = 0; j <= 6; j++) num *= (seq_t)(n - k + j);
        for (idx_t m = 1; m <= 7; m++) den *= (seq_t)(k + m);
        t = t * num / den;
        sum += t;
    }
    return sum;
}
