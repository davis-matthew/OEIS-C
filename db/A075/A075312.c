/*
 * A075312  Products of Wythoff pairs: [n*r]*[n*r^2], where [] is the floor
 *          function and r is the golden ratio, (1+sqrt(5))/2.
 * https://oeis.org/A075312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi) * (floor(n*phi) + n), where phi =
 *            (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t n_seq   = (seq_t)n;
    seq_t n2      = n_seq * n_seq;
    seq_t five_n2 = n2 * (seq_t)5;
    seq_t sqrt5n  = floor_sqrt(five_n2);               /* floor(n*sqrt(5)) */
    seq_t a_val   = (n_seq + sqrt5n) / 2;              /* floor(n*phi)   */
    return a_val * (a_val + n_seq);                   /* product with n+a_val */
}
