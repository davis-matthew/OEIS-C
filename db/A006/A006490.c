/*
 * A006490  a(1) = 1, a(2) = 0; for n > 2, a(n) = n*Fibonacci(n-2) (with the
 *          convention Fibonacci(0)=0, Fibonacci(1)=1).
 * https://oeis.org/A006490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*Fibonacci(n-2) for n > 2; a(1) = 1, a(2) = 0
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 41, u64 n <= 86, u128 n <= 177,
 *            big any n
 * Helpers:   compute_fibonacci
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_fibonacci.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 0;
    return (seq_t)n * compute_fibonacci(n - 2);
}
