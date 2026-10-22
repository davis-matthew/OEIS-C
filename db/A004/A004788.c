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
 * Helpers:   power_in_binom, power_in_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/power_in_binom.h"
#include "../utils/power_in_factorial.h"

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
