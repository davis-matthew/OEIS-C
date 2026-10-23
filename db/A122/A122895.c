/*
 * A122895  Characteristic function of natural numbers with number of divisors
 *          equal to a Fibonacci number.
 * https://oeis.org/A122895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A010056(A000005(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000005(n) = number of divisors of n; */
/* A010056(m) = 1 if m is a Fibonacci number, else 0. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

/* Return true if k is a Fibonacci number. */
static bool is_fibonacci(uint64_t k)
{
    if (k <= 1) return true;   /* 1 is Fibonacci; 0 not needed for divisor count */
    uint64_t a = 1, b = 1;
    while (b < k) {
        uint64_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return b == k;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    uint64_t d = divisor_count_u64(n);       /* d(n) = number of divisors of n */
    return is_fibonacci(d) ? (seq_t)1 : (seq_t)0;
}
