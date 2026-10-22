/*
 * A005124  Numbers k such that 8k + 3 is prime.
 * https://oeis.org/A005124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number k such that 8*k + 3 is prime
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime_check(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    if (n < 9) return true;
    if (n % 3 == 0) return false;
    
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;
    
    while (true) {
        seq_t candidate = (seq_t)8 * k + (seq_t)3;
        if (is_prime_check((uint64_t)candidate)) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k++;
    }
}
