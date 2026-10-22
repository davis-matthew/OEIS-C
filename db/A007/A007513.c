/*
 * A007513  a(n) = initial prime of n consecutive primes such that first and
 *          last have same digit sum.
 * https://oeis.org/A007513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy, nice, look
 * Formula:   a(n) = min { prime(k) | digit_sum(prime(k)) =
 *            digit_sum(prime(k+n-1)) }
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


static uint64_t digit_sum(uint64_t num) {
    uint64_t sum = 0;
    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

static bool is_prime(uint64_t num) {
    if (num < 2) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= num; i += 2) {
        if (num % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    uint64_t n_ull = (uint64_t)n;
    uint64_t primes_target = (n_ull < 100) ? 2000 : (n_ull * 20);
    
    uint64_t *primes = (uint64_t *)malloc(sizeof(uint64_t) * primes_target);
    uint64_t prime_count = 0;
    uint64_t candidate = 2;
    
    while (prime_count < primes_target && candidate < 10000000ULL) {
        if (is_prime(candidate)) {
            primes[prime_count++] = candidate;
        }
        candidate += (candidate == 2) ? 1 : 2;
    }
    
    seq_t result = 0;
    for (uint64_t j = 0; j + n_ull - 1 < prime_count; j++) {
        if (digit_sum(primes[j]) == digit_sum(primes[j + n_ull - 1])) {
            result = (seq_t)primes[j];
            break;
        }
    }
    
    free(primes);
    return result;
}
