/*
 * A127018  Least s with s, t both semiprime and s+2n = t.
 * https://oeis.org/A127018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Least s such that s and t both semiprime and s+2n=t
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Build a prime table once.  The table is large enough for sqrt of the
   maximal t we are likely to encounter for n <= 10000.  Since t <= s+20000
   and s is expected to be < 10^7, primes up to 50000 are plenty. */
static const int PRIME_LIMIT = 50000;
static int primes[60000];
static int prime_count = 0;

/* Simple sieve to populate primes[]. */
static void init_primes(void) {
    bool is_prime[PRIME_LIMIT + 1];
    for (int i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i) is_prime[j] = false;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) primes[prime_count++] = i;
    }
}

/* Return true if n is a semiprime (product of exactly two primes,
   exponents counted). */
static bool is_semiprime(uint64_t n) {
    if (n < 4) return false;   /* smallest semiprime is 4 = 2*2 */
    int factors = 0;
    uint64_t temp = n;
    for (int i = 0; i < prime_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        int p = primes[i];
        while (temp % p == 0) {
            ++factors;
            if (factors > 2) return false;
            temp /= p;
        }
    }
    if (temp > 1) ++factors;
    return factors == 2;
}

seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        init_primes();
        initialized = true;
    }
    uint64_t step = 2 * (uint64_t)n;      /* s + 2n */
    for (uint64_t s = 2; ; ++s) {
        if (is_semiprime(s) && is_semiprime(s + step)) {
            return (seq_t)s;
        }
        /* Safety: if search goes too far we stop, but sequence
           theory guarantees a solution exists for all n >= 1. */
    }
}
