/*
 * A005521  1 + (sum of first n odd primes - n)/2.
 * https://oeis.org/A005521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (sum of first n odd primes - n)/2
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>


static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

static uint64_t *odd_primes = NULL;
static idx_t num_primes = 0;
static idx_t capacity = 0;

static void ensure_primes(idx_t n) {
    if (num_primes >= n) return;
    
    if (capacity == 0) {
        capacity = 1000;
        odd_primes = (uint64_t *)malloc(capacity * sizeof(uint64_t));
    }
    
    uint64_t candidate = (num_primes > 0) ? odd_primes[num_primes - 1] + 2 : 3;
    
    while (num_primes < n) {
        if (capacity <= num_primes) {
            capacity *= 2;
            odd_primes = (uint64_t *)realloc(odd_primes, capacity * sizeof(uint64_t));
        }
        
        if (is_prime(candidate)) {
            odd_primes[num_primes++] = candidate;
        }
        candidate += 2;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    ensure_primes(n);
    
    seq_t sum = 0;
    for (idx_t i = 0; i < n; i++) {
        sum = sum + (seq_t)odd_primes[i];
    }
    
    return 1 + (sum - (seq_t)n) / 2;
}
