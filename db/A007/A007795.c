/*
 * A007795  Juxtapose pairs of primes.
 * https://oeis.org/A007795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Juxtapose the (2n-1)-th and (2n)-th primes as decimal digits
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"


static uint64_t nth_prime(idx_t n) {
    if (n == 1) return 2;
    uint64_t count = 1;
    uint64_t p = 3;
    while (count < n) {
        if (is_prime(p)) count++;
        if (count < n) p += 2;
    }
    return p;
}

static idx_t count_digits(uint64_t x) {
    if (x == 0) return 1;
    idx_t count = 0;
    while (x > 0) {
        count++;
        x /= 10;
    }
    return count;
}

seq_t a(idx_t n) {
    uint64_t p1 = nth_prime(2 * n - 1);
    uint64_t p2 = nth_prime(2 * n);
    
    idx_t digits = count_digits(p2);
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < digits; i++) {
        power = power * (seq_t)10;
    }
    
    seq_t result = (seq_t)p1 * power + (seq_t)p2;
    return result;
}
