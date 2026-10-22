/*
 * A002126  Number of solutions to n=p+q where p and q are primes or zero.
 * https://oeis.org/A002126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = p + q where p and q are primes
 *            or zero
 * Range:     u8 n <= 2729, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool *prime_cache = NULL;
static idx_t cache_size = 0;

static void extend_prime_cache(idx_t n) {
    if (cache_size > n) return;
    
    idx_t new_size = n + 1;
    bool *new_cache = (bool *)malloc(new_size);
    memset(new_cache, 1, new_size);
    
    if (new_size > 0) new_cache[0] = false;
    if (new_size > 1) new_cache[1] = false;
    
    for (idx_t i = 2; i * i < new_size; i++) {
        if (new_cache[i]) {
            for (idx_t j = i * i; j < new_size; j += i) {
                new_cache[j] = false;
            }
        }
    }
    
    if (prime_cache) free(prime_cache);
    prime_cache = new_cache;
    cache_size = new_size;
}

static bool is_prime_or_zero(idx_t x) {
    if (x == 0) return true;
    if (x < cache_size) return prime_cache[x];
    
    if (x == 1) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;
    for (idx_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    extend_prime_cache(n);
    seq_t count = 0;
    for (idx_t i = 0; i <= n; i++) {
        if (is_prime_or_zero(i) && is_prime_or_zero(n - i)) {
            count++;
        }
    }
    return count;
}
