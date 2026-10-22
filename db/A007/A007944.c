/*
 * A007944  a(n) is the largest even number k such that 6, 8, ..., k are sums
 *          of 2 of first n odd primes.
 * https://oeis.org/A007944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the largest even k such that 6, 8, ..., k are all sums
 *            of pairs of the first n odd primes
 * Range:     u8 n <= 33, u16 n <= 3534, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


static bool is_odd_prime(uint64_t num) {
    if (num < 2) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    if (num == 3) return true;
    if (num % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    uint64_t *primes = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    if (primes == NULL) return 0;
    
    idx_t count = 0;
    uint64_t candidate = 3;
    while (count < n) {
        if (is_odd_prime(candidate)) {
            primes[count] = candidate;
            count++;
        }
        candidate += 2;
    }
    
    uint64_t max_result = 2 * primes[n - 1];
    bool *achievable = (bool *)calloc((size_t)(max_result + 1), sizeof(bool));
    if (achievable == NULL) {
        free(primes);
        return 0;
    }
    
    for (idx_t i = 0; i < n; i++) {
        for (idx_t j = i; j < n; j++) {
            uint64_t sum = primes[i] + primes[j];
            if (sum <= max_result) {
                achievable[sum] = true;
            }
        }
    }
    
    seq_t result = 4;
    for (uint64_t k = 6; k <= max_result; k += 2) {
        if (achievable[k]) {
            result = (seq_t)k;
        } else {
            break;
        }
    }
    
    free(primes);
    free(achievable);
    
    return result;
}
