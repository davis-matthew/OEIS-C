/*
 * A086836  On a 3 X 3 board, number of distinct positions of n digits (modulo
 *          rotation/reflection).
 * https://oeis.org/A086836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, fini, full
 * Formula:   a(n) = 1/8 * ( [9]_n + 4*[3]_n + 3*[1]_n ), where [m]_n is the
 *            falling factorial m*(m-1)*…*(m-n+1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   falling_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/falling_factorial.h"

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;  /* offset is 1, below this value is undefined */
    if (n > (idx_t)9) return (seq_t)0;   /* only up to 9 distinct positions are possible */

    seq_t f9 = falling_factorial(9u, n);
    seq_t f3 = falling_factorial(3u, n);
    seq_t f1 = falling_factorial(1u, n);

    seq_t sum = f9 + (seq_t)4 * f3 + (seq_t)3 * f1;
    return sum / (seq_t)8;
}
