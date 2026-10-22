/*
 * A002082  2nd differences are periodic.
 * https://oeis.org/A002082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   2nd differences have period 10: [2,4,0,6,8,8,6,0,4,2]. For n =
 *            10*Q + R with 0 <= R <= 9: a(n) = a(R) + 20*Q*(10*Q - 1) +
 *            40*Q*R, a(0..9) = 2, 2, 4, 10, 16, 28, 48, 76, 110, 144
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const unsigned base[10] = {2, 2, 4, 10, 16, 28, 48, 76, 110, 144};
    idx_t Q = n / 10;
    unsigned R = (unsigned)(n % 10);
    seq_t q = (seq_t)Q;
    seq_t r = (seq_t)base[R];
    if (Q > 0) {
        r += (seq_t)20 * q * ((seq_t)10 * q - 1);
        r += (seq_t)40 * q * (seq_t)R;
    }
    return r;
}
