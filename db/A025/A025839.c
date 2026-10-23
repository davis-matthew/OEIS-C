/*
 * A025839  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^11)).
 * https://oeis.org/A025839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 19*n + 232 * + 37*((n+2 mod 3) - (n+1 mod
 *            3)) * + 14*(2*((n+3 mod 5)) - 2*(n mod 5) * + ((n+1 mod 5)) -
 *            ((n+2 mod 5)))) / 330)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute using a wider unsigned integer for safety */
    uint64_t nn = (uint64_t)n;
    uint64_t mod3_n_plus2 = (nn + 2) % 3;
    uint64_t mod3_n_plus1 = (nn + 1) % 3;
    uint64_t mod5_n_plus3 = (nn + 3) % 5;
    uint64_t mod5_n       = nn % 5;
    uint64_t mod5_n_plus1 = (nn + 1) % 5;
    uint64_t mod5_n_plus2 = (nn + 2) % 5;

    uint64_t num = nn * nn
                 + 19 * nn
                 + 232
                 + 37 * ((mod3_n_plus2 - mod3_n_plus1))
                 + 14 * ( 2 * mod5_n_plus3
                          - 2 * mod5_n
                          + mod5_n_plus1
                          - mod5_n_plus2 );

    return (seq_t)(num / 330);
}
