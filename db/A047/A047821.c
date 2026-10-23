/*
 * A047821  Becomes prime after exactly 2 iterations of f(x) = sum of prime
 *          factors of x.
 * https://oeis.org/A047821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047821: numbers x for which the iterated sum of distinct prime factors
   reaches a prime after exactly two applications, but not after one:
   f(x)  = sum of distinct prime factors of x
   f(f(x)) is prime   and   f(x) is composite (or <=1). */

#include <stdint.h>
#include <stdbool.h>

/* helper: sum of distinct prime factors of n */
static uint64_t sum_distinct_prime_factors(uint64_t n) {
    if (n < 2) return 0;
    uint64_t sum = 0;
    uint64_t d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            sum += d;
            while (n % d == 0) n /= d;
        }
        d = (d == 2) ? 3 : d + 2;  /* skip even numbers after 2 */
    }
    if (n > 1) sum += n;  /* remaining prime factor */
    return sum;
}

/* helper: primality test (trial division) */
static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n) {
    idx_t found = 0;
    uint64_t x = 1;
    while (found < n) {
        uint64_t fx = sum_distinct_prime_factors(x);
        if (fx > 1 && !is_prime(fx)) {
            uint64_t ffx = sum_distinct_prime_factors(fx);
            if (is_prime(ffx)) {
                found++;
                if (found == n) return (seq_t)x;
            }
        }
        x++;
    }
    /* unreachable */
    return (seq_t)0;
}
