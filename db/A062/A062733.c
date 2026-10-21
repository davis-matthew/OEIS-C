/*
 * A062733  Maximal degree of an irreducible representation of the group
 *          SL(n,2) (the group of nonsingular n X n matrices over GF(2) ).
 * https://oeis.org/A062733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^((n-2)*(n-3)/2) * (2^n - 1) * (2^(n-1) - 1) / 3 for n
 *            >= 4 Special cases: a(1)=1, a(2)=2, a(3)=8
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow2_big(idx_t e) {
    seq_t result = 1;
    seq_t base = 2;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 8;
    idx_t exp = ((n - 2) * (n - 3)) / 2;
    seq_t pow_exp = pow2_big(exp);
    seq_t two_n = pow2_big(n);
    seq_t two_n_minus1 = two_n - 1;
    seq_t two_n_minus2 = pow2_big(n - 1) - 1;
    seq_t result = pow_exp * two_n_minus1;
    result *= two_n_minus2;
    result /= 3;
    return result;
}
