/*
 * A031922  a(n) = prime(1000 * n).
 * https://oeis.org/A031922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(1000*n)
 * Range:     u8 none, u16 n <= 6, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/* Internal state for the prime sieve                                     */
/* --------------------------------------------------------------------- */
static unsigned char *sieve_bits = NULL;   /* bitset of odd composites  */
static size_t       max_limit = 0;         /* current sieve upper bound */
static uint64_t    *prime_cache = NULL;    /* list of primes found */
static size_t       prime_cnt   = 0;        /* number of primes stored */

/* --------------------------------------------------------------------- */
/* Estimate an upper bound for the k-th prime (valid for k >= 6)          */
/* --------------------------------------------------------------------- */
static size_t upper_bound_prime(size_t k)
{
    if (k < 6) return 15;
    double dk = (double)k;
    double bound = dk * (log(dk) + log(log(dk))) + 3.0;
    return (size_t)bound;
}

/* --------------------------------------------------------------------- */
/* Build a sieve up to 'limit' (inclusive) and rebuild the prime cache   */
/* --------------------------------------------------------------------- */
static void generate_primes_up_to(size_t limit)
{
    /* free previous bitset */
    free(sieve_bits);

    /* one byte per 8 odd numbers -> 16 numbers per byte */
    size_t bits_len = (limit >> 4) + 1;
    sieve_bits = (unsigned char*)malloc(bits_len);
    if (!sieve_bits) exit(1);
    memset(sieve_bits, 0, bits_len);

    /* Sieve of Eratosthenes for odd numbers */
    size_t sqrt_limit = (size_t)sqrt((double)limit);
    for (size_t i = 3; i <= sqrt_limit; i += 2) {
        size_t idx = (i - 3) >> 1;
        if (!(sieve_bits[idx >> 3] & (1u << (idx & 7)))) {
            for (size_t j = i * i; j <= limit; j += i << 1) {
                size_t jx = (j - 3) >> 1;
                sieve_bits[jx >> 3] |= 1u << (jx & 7);
            }
        }
    }

    /* Rebuild prime cache */
    free(prime_cache);
    prime_cnt = 0;
    size_t capacity = limit / 10 + 10;   /* rough estimate of prime count */
    prime_cache = (uint64_t*)malloc(capacity * sizeof(uint64_t));
    if (!prime_cache) exit(1);

    /* first prime */
    prime_cache[prime_cnt++] = 2;

    for (size_t n = 3; n <= limit; n += 2) {
        size_t id = (n - 3) >> 1;
        if (!(sieve_bits[id >> 3] & (1u << (id & 7)))) {
            if (prime_cnt == capacity) {
                capacity *= 2;
                prime_cache = (uint64_t*)realloc(prime_cache,
                                                capacity * sizeof(uint64_t));
                if (!prime_cache) exit(1);
            }
            prime_cache[prime_cnt++] = n;
        }
    }
    max_limit = limit;
}

/* --------------------------------------------------------------------- */
/* Ensure that at least 'k' primes are cached                            */
/* --------------------------------------------------------------------- */
static void ensure_k_primes(size_t k)
{
    if (prime_cnt >= k) return;

    size_t limit = upper_bound_prime(k);
    if (limit < 16) limit = 16;

    while (prime_cnt < k) {
        /* If the current limit is insufficient, it will be updated in */
        /* generate_primes_up_to when the loop repeats.               */
        generate_primes_up_to(limit);
        if (prime_cnt < k) {
            /* Need a larger limit – double the current max */
            limit = max_limit * 2;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* k = 1000 * n (index into primes) */
    size_t k = (size_t)n * 1000ULL;

    ensure_k_primes(k);

    /* Return the (k-1)-th prime (0‑based) as seq_t */
    return (seq_t)prime_cache[k - 1];
}
