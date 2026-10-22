/*
 * A004398  a(n) = Fibonacci(n+1) + prime(n).
 * https://oeis.org/A004398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+1) + prime(n)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(seq_t num) {
    if (num < 2) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    
    for (seq_t i = 3; i * i <= num; i += 2) {
        if (num % i == 0) return false;
    }
    
    return true;
}

static seq_t nth_prime(idx_t n) {
    if (n == 1) return 2;
    
    seq_t candidate = 3;
    idx_t count = 1;
    
    while (count < n) {
        if (is_prime(candidate)) {
            count++;
        }
        if (count < n) {
            candidate += 2;
        }
    }
    
    return candidate;
}

static seq_t fibonacci(idx_t n) {
    if (n == 1 || n == 2) return 1;
    
    seq_t prev = 1, curr = 1;
    for (idx_t i = 3; i <= n; i++) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    
    return curr;
}

seq_t a(idx_t n) {
    return fibonacci(n + 1) + nth_prime(n);
}
