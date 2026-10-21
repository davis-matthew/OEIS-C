/*
 * A083339  a(n) is the number of distinct prime factors of n that occur in
 *          partitions into two primes when n is even and into three primes
 *          when n is odd.
 * https://oeis.org/A083339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2048, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   modmul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: if n is even, a(n) = 1 if n/2 is prime, else 0;
 * if n is odd, a(n) = 0 if n is prime, else the number of distinct prime
 * factors of n.
 *
 * The implementation uses a deterministic Miller–Rabin test for 64–bit
 * integers and trial division with primes up to 1 000 000 for factorisation
 * of odd composites.  All arithmetic fits in 64 bits.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/modmul64.h"

/* ------------------------------------------------------------ */
/* Miller–Rabin primality test (deterministic for 64 bits)      */

static inline uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = modmul64(result, base, mod);
        base = modmul64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool isPrime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* Bases that work for all 64‑bit integers */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = modmul64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* Sieve of Eratosthenes up to 1 000 000 for factorisation       */

#define SIEVE_LIMIT 1000000

static uint32_t primes[SIEVE_LIMIT/10 + 1000];
static size_t prime_count = 0;
static bool primes_built = false;

static void build_primes(void) {
    if (primes_built) return;
    bool is_composite[SIEVE_LIMIT + 1] = {0};
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_count < sizeof(primes)/sizeof(primes[0]))
                primes[prime_count++] = i;
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    primes_built = true;
}

static int count_distinct_prime_factors(uint64_t n) {
    if (n <= 1) return 0;
    build_primes();
    int count = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            ++count;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) ++count;   /* remaining prime factor */
    return count;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                         */

seq_t a(idx_t n)
{
    seq_t res = 0;
    if ((n & 1) == 0) {          /* even n */
        idx_t k = n >> 1;
        res = isPrime_uint64((uint64_t)k) ? (seq_t)1 : (seq_t)0;
    } else {                     /* odd n */
        if (isPrime_uint64((uint64_t)n))
            res = 0;
        else
            res = (seq_t)count_distinct_prime_factors((uint64_t)n);
    }
    return res;
}
