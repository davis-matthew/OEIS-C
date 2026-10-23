/*
 * A138145  Palindromes formed from the reflected decimal expansion of the
 *          concatenation of 1, 1, 1 and infinite 0's.
 * https://oeis.org/A138145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 111 + 111*10^(n-3) for n ≥ 6 * Base cases: a(1)=1,
 *            a(2)=11, a(3)=111, * a(4)=1111, a(5)=11111
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)11,
        (seq_t)111,
        (seq_t)1111,
        (seq_t)11111
    };
    if (n <= 5) return init[n-1];
    seq_t p = pow10(n - 3);          /* 10^(n-3) */
    return mul_small(p, (unsigned)111) + (seq_t)111;
}
