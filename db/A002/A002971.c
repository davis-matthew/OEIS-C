/*
 * A002971  Numbers k such that 4*k^2 + 25 is prime.
 * https://oeis.org/A002971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that 4*k^2 + 25 is prime
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime_seq(seq_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    
    for (seq_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t k = 0;
    
    while (count < (seq_t)n) {
        k++;
        seq_t val = 4 * k * k + 25;
        if (is_prime_seq(val)) {
            count++;
        }
    }
    
    return k;
}
