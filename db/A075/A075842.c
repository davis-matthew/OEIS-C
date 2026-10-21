/*
 * A075842  n 1's followed by n.
 * https://oeis.org/A075842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = ((10^n - 1)/9) * 10^{digits(n)} + n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n) {
    /* Construct (10^n - 1)/9 */
    seq_t ones = pow10(n);
    ones -= (seq_t)1;
    ones /= (seq_t)9;

    /* Multiply by 10^{digits(n)} and add n */
    int L = digits(n);
    seq_t powL = pow10((idx_t)L);
    seq_t res = ones * powL;
    res += (seq_t)n;
    return res;
}
