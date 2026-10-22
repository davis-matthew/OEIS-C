/*
 * A006224  Rabbytes: group eight successive Fibonacci numbers in binary and
 *          translate to decimal.
 * https://oeis.org/A006224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation in binary of F(max(0,n-7)), ..., F(n)
 *            (eight successive Fibonacci numbers), read as decimal; F(0)=0,
 *            F(1)=1, F(k)=F(k-1)+F(k-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t start = (n >= 7) ? n - 7 : 0;
    seq_t f0 = 0, f1 = 1;
    for (idx_t i = 0; i < start; i++) {
        seq_t t = f0 + f1;
        f0 = f1;
        f1 = t;
    }
    seq_t result = 0;
    for (idx_t i = start; i <= n; i++) {
        int bits = 0;
        seq_t temp = f0;
        do {
            bits++;
            temp >>= 1;
        } while (temp > 0);
        result = (result << bits) | f0;
        seq_t t = f0 + f1;
        f0 = f1;
        f1 = t;
    }
    return result;
}
