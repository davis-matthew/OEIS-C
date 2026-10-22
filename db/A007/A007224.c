/*
 * A007224  Number of distinct perforation patterns for deriving (v,b) =
 *          (n+3,n) punctured convolutional codes from (2,1).
 * https://oeis.org/A007224
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*(binomial(2*n, n+3) + 2*[(n mod 3) ==
 *            0]*binomial(2*n/3, n/3 + 1))
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   a007224_bin128, a007224_binS, a007224_gcd64, seq_is_big,
 *            u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a007224_bin128.h"
#include "../utils/a007224_binS.h"
#include "../utils/a007224_gcd64.h"

seq_t a(idx_t n) {
    if (seq_is_big()) {
        seq_t s = a007224_binS(2 * n, n + 3);
        if (n % 3 == 0) {
            s += 2 * a007224_binS(2 * n / 3, n / 3 + 1);
        }
        return s / (seq_t)n;
    } else {
        unsigned __int128 s = a007224_bin128(2 * n, n + 3);
        if (n % 3 == 0) {
            s += 2 * a007224_bin128(2 * n / 3, n / 3 + 1);
        }
        return u128_to_seq(s / n);
    }
}
