/*
 * A004399  a(n) = Fibonacci(n+2) + prime(n).
 * https://oeis.org/A004399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+2) + prime(n)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(uint64_t p) {
    if (p < 2) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= p; i += 2) {
        if (p % i == 0) return false;
    }
    return true;
}

static seq_t fibonacci(idx_t k) {
    if (k == 1) return 1;
    if (k == 2) return 1;
    
    seq_t a = 1, b = 1;
    for (idx_t i = 3; i <= k; i++) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

static seq_t nth_prime(idx_t n) {
    if (n == 1) return 2;
    
    idx_t count = 1;
    uint64_t candidate = 3;
    
    while (true) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate += 2;
    }
}

seq_t a(idx_t n) {
    seq_t fib = fibonacci(n + 2);
    seq_t prime = nth_prime(n);
    return fib + prime;
}
