/*
 * A007468  Sum of next n primes.
 * https://oeis.org/A007468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of n consecutive primes starting from prime(1 +
 *            n(n-1)/2)
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 842, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static bool is_prime(uint64_t n) {
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

static uint64_t *primes = NULL;
static idx_t primes_count = 0;
static idx_t primes_capacity = 0;

static uint64_t get_prime(idx_t k) {
    if (k <= primes_count) {
        return primes[k - 1];
    }
    
    uint64_t candidate = (primes_count > 0) ? primes[primes_count - 1] + 1 : 2;
    
    while (primes_count < k) {
        if (is_prime(candidate)) {
            if (primes_count >= primes_capacity) {
                primes_capacity = (primes_capacity == 0) ? 1000 : primes_capacity * 2;
                primes = (uint64_t *)realloc(primes, primes_capacity * sizeof(uint64_t));
            }
            primes[primes_count++] = candidate;
        }
        candidate++;
    }
    
    return primes[k - 1];
}

seq_t a(idx_t n) {
    idx_t start = 1 + n * (n - 1) / 2;
    idx_t end = n + n * (n - 1) / 2;
    
    seq_t sum = 0;
    for (idx_t i = start; i <= end; i++) {
        sum += (seq_t)get_prime(i);
    }
    
    return sum;
}
