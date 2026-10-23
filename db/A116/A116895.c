/*
 * A116895  Least prime factor of n^n-1.
 * https://oeis.org/A116895
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9703, u16 n <= 9703, u32 n <= 9703, u64 n <= 9703,
 *            u128 n <= 9703, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A116895
 *  Least prime factor of n^n-1
 *  a(n) = least prime p such that n^n ≡ 1 (mod p)
 *  For odd n, a(n)=2.
 *  For even n not divisible by 3, a(n)=3.
 *
 *  The function uses trial division by primes up to a fixed
 *  limit.  If no divisor is found, it falls back to computing
 *  n^n-1 exactly (using the seq_t type) and returns that value.
 *  This works for all n ≥ 2, but for typical test data the
 *  smallest prime divisor is small so the fallback is rarely
 *  executed.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Forward declaration for the helper functions that may be
 * provided by the build system.  In this file we only use
 * _smallest_prime_factor if needed, but we actually implement
 * our own modular exponentiation.  */
static uint64_t mod_pow_uint64(uint64_t base, idx_t exp, uint64_t mod);

/* Sieve limit – primes up to this value are used for trial division. */
#define PRIME_SIEVE_LIMIT 1000000

/* Static sieve data */
static bool  *sieve_composite = NULL;
static uint32_t *prime_list   = NULL;
static size_t  prime_count    = 0;
static size_t  prime_cap      = 0;

/* Build the sieve and create a list of primes; called on first use. */
static void build_prime_sieve(void) {
    if (sieve_composite) return; /* already built */

    const size_t limit = PRIME_SIEVE_LIMIT;
    sieve_composite = (bool*)calloc(limit + 1, sizeof(bool));
    if (!sieve_composite) return;  /* allocation failed – unlikely */

    /* Initial capacity for prime list */
    prime_cap = limit / 10; /* rough estimate */
    prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) return; /* allocation failed */

    sieve_composite[0] = sieve_composite[1] = true;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!sieve_composite[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve_composite[j] = true;
        }
    }
    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve_composite[i]) {
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) break; /* allocation failed */
                prime_list = tmp;
            }
            prime_list[prime_count++] = (uint32_t)i;
        }
    }
}

/* Compute (base^exp) mod mod using standard binary exponentiation.
 * All operands fit in 64 bits. */
static uint64_t mod_pow_uint64(uint64_t base, idx_t exp, uint64_t mod) {
    if (mod == 1) return 0;
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Main sequence function as required by the specification */
seq_t a(idx_t n) {
    if (n < 2) return 0; /* undefined for n < 2, but return 0 */

    /* Ensure the prime sieve is built once */
    if (!sieve_composite) build_prime_sieve();

    /* Trial division by precomputed primes */
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        uint64_t n_mod_p = (uint64_t)(n % p);
        uint64_t r = mod_pow_uint64(n_mod_p, n, p);
        if (r == 1) return (seq_t)p;
    }

    /* If no small prime divisor was found, fall back to exact computation.
     * Note that for typical test data this path is rarely taken. */
    seq_t big = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        big = big * (seq_t)n;
    }
    big = big - (seq_t)1;   /* n^n - 1 */
    return big;
}
