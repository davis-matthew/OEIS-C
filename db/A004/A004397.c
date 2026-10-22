/*
 * A004397  a(n) = prime(n) + Fibonacci(n).
 * https://oeis.org/A004397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + Fibonacci(n)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   get_nth_fibonacci, get_nth_prime, is_prime_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_nth_fibonacci.h"
#include "../utils/get_nth_prime.h"
#include "../utils/is_prime_helper.h"

seq_t a(idx_t n) {
    uint64_t prime = get_nth_prime(n);
    seq_t fib = get_nth_fibonacci(n);
    return (seq_t)prime + fib;
}
