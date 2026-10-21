/*
 * A078795  Concatenate first n triangular numbers.
 * https://oeis.org/A078795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = concat_{k=1..n} T_k         where   T_k = k(k+1)/2
 * Computes the concatenation of the first n triangular numbers.
 */

#include <stdint.h>
#include "../utils/pow10.h"      /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h"  /* int digit_len(seq_t x); */

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset 1 – a(1) is the first term */

    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t tri = (seq_t)i * (seq_t)(i + 1) / 2;     /* triangular number T_i */
        idx_t d = digit_len(tri);                     /* number of decimal digits of T_i */
        res = res * pow10(d) + tri;                   /* concatenate T_i */
    }
    return res;
}
