/*
 * A133022  Product of n-th Fibonacci number and n-th Fibonacci number written
 *          backwards.
 * https://oeis.org/A133022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A000045(n) * A004091(n)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 47, u128 n <= 92,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

static seq_t fib_of(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t prev = 0;
    seq_t curr = 1;
    for (idx_t i = 1; i < n; ++i) {
        seq_t tmp = prev + curr;
        prev = curr;
        curr = tmp;
    }
    return curr;
}

seq_t a(idx_t n)
{
    seq_t f  = fib_of(n);
    seq_t rf = reverse_decimal(f);
    return f * rf;
}
