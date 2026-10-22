/*
 * A000430  Primes and squares of primes.
 * https://oeis.org/A000430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n-th number that is either prime or square of a prime
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>


#define SIEVE_LIMIT 300000

static bool sieve[SIEVE_LIMIT + 1];
static int prime_count[SIEVE_LIMIT + 1];
static bool sieve_initialized = false;

static void init_sieve(void) {
    if (sieve_initialized) return;
    
    memset(sieve, 1, sizeof(sieve));
    sieve[0] = sieve[1] = false;
    
    for (int i = 2; (long long)i * i <= SIEVE_LIMIT; i++) {
        if (sieve[i]) {
            for (long long j = (long long)i * i; j <= SIEVE_LIMIT; j += i) {
                sieve[(int)j] = false;
            }
        }
    }
    
    prime_count[0] = 0;
    for (int i = 1; i <= SIEVE_LIMIT; i++) {
        prime_count[i] = prime_count[i - 1] + (sieve[i] ? 1 : 0);
    }
    
    sieve_initialized = true;
}

static long long count_terms_up_to(long long limit) {
    if (limit > SIEVE_LIMIT) limit = SIEVE_LIMIT;
    if (limit < 2) return 0;
    
    long long count = prime_count[(int)limit];
    
    for (long long p = 2; p * p <= limit; p++) {
        if (sieve[(int)p]) count++;
    }
    
    return count;
}

seq_t a(idx_t n) {
    init_sieve();
    
    long long left = 2, right = (long long)n * 30;
    if (right > SIEVE_LIMIT) right = SIEVE_LIMIT;
    
    while (left < right) {
        long long mid = left + (right - left) / 2;
        if (count_terms_up_to(mid) < (long long)n) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    return (seq_t)left;
}
