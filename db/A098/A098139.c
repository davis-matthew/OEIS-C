/*
 * A098139  a(n) = 2^p + 3^p + 5^p + 7^p where p = prime(n).
 * https://oeis.org/A098139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^p + 3^p + 5^p + 7^p where p = prime(n) (1‑based)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"  /* seq_pow(seq_t base, idx_t exp) */

static seq_t *prime_list = NULL;          /* array of primes (1‑based) */
static idx_t  prime_count = 0;            /* number of primes stored   */
static idx_t  prime_cap   = 0;            /* capacity of prime_list    */

/* simple integer square root for 64‑bit values (needed for primality test) */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

/* trial‑division primality test using already known primes */
static int is_prime_candidate(seq_t value)
{
    if (value < 2) return 0;
    if (value % 2 == 0) return value == 2;

    uint64_t root = isqrt_uint64((uint64_t)value);
    for (idx_t i = 0; i < prime_count && prime_list[i] <= root; ++i) {
        if (value % prime_list[i] == 0) return 0;
    }
    return 1;
}

/* ensure that prime_list contains at least n_needed primes */
static void ensure_primes(idx_t n_needed)
{
    if (prime_count >= n_needed) return;

    if (!prime_list) {
        prime_cap   = 128;
        prime_list  = (seq_t *)calloc(prime_cap, sizeof(seq_t));
    }

    seq_t candidate = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
    while (prime_count < n_needed) {
        if (is_prime_candidate((seq_t)candidate)) {
            if (prime_count >= prime_cap) {
                prime_cap  *= 2;
                prime_list = (seq_t *)realloc(prime_list, prime_cap * sizeof(seq_t));
            }
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) {
        /* behavior for n < 1 is undefined – return 0 as a sane default */
        return (seq_t)0;
    }

    /* get the nth prime (primes[0] = 2) */
    ensure_primes(n);
    seq_t p = prime_list[n - 1];

    /* compute 2^p + 3^p + 5^p + 7^p */
    seq_t term2 = seq_pow((seq_t)2, (idx_t)p);
    seq_t term3 = seq_pow((seq_t)3, (idx_t)p);
    seq_t term5 = seq_pow((seq_t)5, (idx_t)p);
    seq_t term7 = seq_pow((seq_t)7, (idx_t)p);

    return term2 + term3 + term5 + term7;
}
