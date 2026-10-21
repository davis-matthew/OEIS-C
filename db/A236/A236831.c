/*
 * A236831  Number of ordered ways to write n = p + q with q > 0 such that p,
 *          p + 2 and p + prime(q) + 1 are all prime.
 * https://oeis.org/A236831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of pairs (p,q) with p+q=n,q>0 such that p, p+2 and
 *            p+prime(q)+1 are primes. prime(q) is the q‑th prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIEVE_LIMIT 200000          /* sufficient for n up to ≈10000 */

static bool sieve_built = false;
static unsigned char *is_prime_flag = NULL;   /* 0 composite, 1 prime */
static uint32_t *prime_list = NULL;           /* prime numbers in order */
static size_t prime_count = 0;

/* build a prime sieve up to SIEVE_LIMIT and fill prime_list */
static void build_sieve(void)
{
    size_t i, j;
    size_t limit = SIEVE_LIMIT;

    is_prime_flag = (unsigned char *)malloc((limit + 1) * sizeof(unsigned char));
    if (!is_prime_flag) abort();
    for (i = 0; i <= limit; ++i) is_prime_flag[i] = 1;   /* assume all prime */
    is_prime_flag[0] = is_prime_flag[1] = 0;              /* 0,1 are composite */

    for (i = 2; i * i <= limit; ++i) {
        if (is_prime_flag[i]) {
            for (j = i * i; j <= limit; j += i) {
                is_prime_flag[j] = 0;
            }
        }
    }

    /* allocate an array big enough for all primes up to SIEVE_LIMIT */
    size_t cap = 20000;   /* primes below 200000 ≈ 17984 */
    prime_list = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!prime_list) abort();

    prime_count = 0;
    for (i = 2; i <= limit; ++i) {
        if (is_prime_flag[i]) {
            if (prime_count == cap) {
                cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_count++] = (uint32_t)i;
        }
    }
}

/* main sequence function, external linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    if (!sieve_built) {
        build_sieve();
        sieve_built = true;
    }

    seq_t count = 0;

    for (idx_t q = 1; q < n; ++q) {
        idx_t p = n - q;
        if (p + 2 > SIEVE_LIMIT) continue;          /* safety guard */

        if (!is_prime_flag[p]) continue;
        if (!is_prime_flag[p + 2]) continue;

        if ((size_t)(q - 1) >= prime_count) break; /* q too large for prime list */

        idx_t prime_q = (idx_t)prime_list[q - 1];
        idx_t candidate = p + prime_q + 1;

        if (candidate > SIEVE_LIMIT) continue;      /* safety guard */

        if (!is_prime_flag[candidate]) continue;

        ++count;
    }

    return count;
}
