/*
 * A004397  a(n) = prime(n) + Fibonacci(n).
 * https://oeis.org/A004397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + Fibonacci(n)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   is_prime_u64, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/nth_prime.h"

static seq_t get_nth_fibonacci(idx_t n) {
    if (n == 1 || n == 2) return 1;
    
    seq_t a = 1, b = 1;
    for (idx_t i = 3; i <= n; i++) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

seq_t a(idx_t n) {
    uint64_t prime = nth_prime(n);
    seq_t fib = get_nth_fibonacci(n);
    return (seq_t)prime + fib;
}
