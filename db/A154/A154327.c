/*
 * A154327  Diagonal sums of number triangle A132046.
 * https://oeis.org/A154327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0^n - (3 + (-1)^n)/2 + 2*Fib(n+1)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    seq_t val = fib((idx_t)n + 1);
    val = val * ((seq_t)2);           /* 2*Fib(n+1) */
    /* subtract (3 + (-1)^n)/2: 2 if n is even, 1 if n is odd */
    if ((n & 1) == 0) {
        val -= ((seq_t)2);
    } else {
        val -= ((seq_t)1);
    }
    /* add 0^n: 1 if n == 0, 0 otherwise */
    if (n == 0) {
        val += ((seq_t)1);
    }
    return val;
}
