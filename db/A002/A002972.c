/*
 * A002972  a(n) is the odd member of {x,y}, where x^2 + y^2 is the n-th prime
 *          of the form 4i+1.
 * https://oeis.org/A002972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the odd member of {x,y}, where x^2 + y^2 is the n-th
 *            prime of the form 4k+1
 * Range:     u8 n <= 3295, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t isqrt_u64(uint64_t x) {
    if (x == 0) return 0;
    uint64_t s = x;
    uint64_t prev;
    do {
        prev = s;
        s = (s + x / s) / 2;
    } while (s < prev);
    return s;
}

static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    if (n < 9) return true;
    if (n % 3 == 0) return false;
    
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

static bool find_sum_of_two_squares(uint64_t p, uint64_t *x, uint64_t *y) {
    uint64_t max_i = isqrt_u64(p / 2) + 1;
    
    for (uint64_t i = 1; i <= max_i; i++) {
        uint64_t remainder = p - i * i;
        uint64_t j = isqrt_u64(remainder);
        
        if (j * j == remainder) {
            *x = i;
            *y = j;
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t candidate = 5;
    
    while (true) {
        if (is_prime_u64(candidate) && candidate % 4 == 1) {
            count++;
            if (count == (uint64_t)n) {
                uint64_t x, y;
                find_sum_of_two_squares(candidate, &x, &y);
                return (x % 2 == 1) ? (seq_t)x : (seq_t)y;
            }
        }
        candidate += 2;
    }
}
