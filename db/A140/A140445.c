/*
 * A140445  List of prime pairs of form p, p + 10.
 * https://oeis.org/A140445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 2308, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = prime such that p+10 is prime, enumerated as
 *  p_k, p_k+10 for the k-th such prime p_k in increasing order.
 *  a(n) = p_{(n+1)/2}  if n is odd
 *  a(n) = p_{(n/2)}+10 if n is even
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool is_prime_u64(uint64_t n, uint32_t *primes, size_t prime_cnt)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static const idx_t LIMIT = 20000000;    /* 20 million is enough for 10000 terms */
    static unsigned char *sieve = NULL;     /* 0 = composite, 1 = prime */
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;
    static uint32_t *pair_primes = NULL;
    static size_t pair_cnt = 0;

    if (!initialized) {
        /* Sieve of Eratosthenes up to LIMIT */
        sieve = (unsigned char *)malloc((size_t)(LIMIT + 1));
        memset(sieve, 1, (size_t)(LIMIT + 1));
        sieve[0] = sieve[1] = 0;
        for (idx_t i = 2; i * i <= LIMIT; ++i) {
            if (sieve[i]) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    sieve[j] = 0;
            }
        }

        /* Collect primes */
        for (idx_t i = 2; i <= LIMIT; ++i)
            if (sieve[i]) primes_cnt++;
        primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
        size_t pc = 0;
        for (idx_t i = 2; i <= LIMIT; ++i)
            if (sieve[i]) primes[pc++] = (uint32_t)i;

        /* Collect primes p such that p+10 is also prime */
        size_t cap = 16;
        pair_primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            uint64_t q = (uint64_t)p + 10ULL;
            bool q_prime;
            if (q <= (uint64_t)LIMIT)
                q_prime = sieve[(size_t)q];
            else
                q_prime = is_prime_u64(q, primes, primes_cnt);
            if (q_prime) {
                if (pair_cnt == cap) {
                    cap <<= 1;
                    pair_primes = (uint32_t *)realloc(pair_primes, cap * sizeof(uint32_t));
                }
                pair_primes[pair_cnt++] = p;
            }
        }
        initialized = true;
    }

    if (n == 0) return 0;                /* undefined for n<1, safe fallback */
    idx_t k = (n + 1) / 2;                /* which pair we need   */
    if (k > pair_cnt) return 0;           /* not enough precomputation */
    uint32_t base = pair_primes[k - 1];
    if (n & 1U)
        return (seq_t)base;
    else
        return (seq_t)((uint64_t)base + 10ULL);
}
