/*
 * A087713  Greatest prime factor of the product of the neighbors of the n-th
 *          prime.
 * https://oeis.org/A087713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest prime factor of (prime(n)-1)(prime(n)+1)
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t n); */

static uint64_t
largest_prime_factor_uint64(uint64_t n) {
    if (n < 2)
        return 1;
    uint64_t largest = 1;
    while ((n & 1) == 0) {          /* factor 2 */
        largest = 2;
        n >>= 1;
    }
    for (uint64_t i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            largest = i;
            n /= i;
        }
    }
    if (n > 1)
        largest = n;                 /* remaining prime factor */
    return largest;
}

seq_t a(idx_t n) {
    /* nth prime (1‑indexed) */
    uint32_t p = get_prime(n);

    /* neighbors of the prime */
    uint64_t left  = (uint64_t)p - 1;
    uint64_t right = (uint64_t)p + 1;

    /* largest prime factors of the neighbors */
    uint64_t g_left  = largest_prime_factor_uint64(left);
    uint64_t g_right = largest_prime_factor_uint64(right);

    /* greatest prime factor of the product */
    return (seq_t)(g_left > g_right ? g_left : g_right);
}
