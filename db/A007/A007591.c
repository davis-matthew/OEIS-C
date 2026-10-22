/*
 * A007591  Numbers k such that k^2 + 4 is prime.
 * https://oeis.org/A007591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   k-th number such that k^2 + 4 is prime (search)
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime_seq(seq_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    
    seq_t i;
    for (i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    seq_t k = 1;
    idx_t count = 0;
    
    while (1) {
        if (is_prime_seq(k * k + 4)) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k += 2;
    }
}
