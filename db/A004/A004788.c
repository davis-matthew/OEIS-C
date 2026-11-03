/*
 * A004788  Number of distinct prime divisors of the numbers in row n of
 *          Pascal's triangle.
 * https://oeis.org/A004788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(A001142(n)), count distinct primes dividing lcm
 *            of binomial coefficients in row n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 89 terms through n = 986 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static idx_t power_in_factorial(idx_t n, idx_t p) {
    idx_t power = 0;
    idx_t pk = p;
    while (pk <= n) {
        power += n / pk;
        if (pk > n / p) break;
        pk *= p;
    }
    return power;
}

static idx_t power_in_binom(idx_t n, idx_t k, idx_t p) {
    return power_in_factorial(n, p) - power_in_factorial(k, p) - power_in_factorial(n - k, p);
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    for (idx_t i = 0; i <= n; i++) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    
    for (idx_t i = 2; i * i <= n; i++) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    int count = 0;
    for (idx_t p = 2; p <= n; p++) {
        if (is_prime[p]) {
            for (idx_t j = 0; j <= n / 2; j++) {
                if (power_in_binom(n, j, p) > 0) {
                    count++;
                    break;
                }
            }
        }
    }
    
    free(is_prime);
    return (seq_t)count;
}
