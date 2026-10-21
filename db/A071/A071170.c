/*
 * A071170  n-th prime reduced modulo sigma(n).
 * https://oeis.org/A071170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Mod(prime(n), sigma(n)) (OEIS A071170)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/sieve_primes.h"

/* static cache of primes */
static idx_t *prime_list   = NULL;   /* primes, 0‑based */
static size_t prime_list_cnt = 0;    /* number of primes stored */
static size_t prime_list_cap = 0;    /* allocated capacity */

/* Ensure that prime_list contains at least n primes (n ≥ 1). */
static void ensure_prime_list(idx_t n)
{
    if (prime_list_cnt >= n)
        return;

    /* Rough upper bound for the n‑th prime (Rosser's theorem). */
    double dn   = (double)n;
    double bound;
    if (n < 6)
        bound = 15.0;
    else
        bound = dn * (log(dn) + log(log(dn))) + 3.0;

    idx_t limit = (idx_t)bound;

    idx_t *new_primes = NULL;
    size_t new_cnt = 0;
    sieve_primes(limit, &new_primes, &new_cnt);

    if (new_primes == NULL)
        return;            /* unlikely, but avoid crash */

    /* Allocate (or reallocate) the cache to hold the returned primes. */
    if (prime_list == NULL) {
        prime_list = (idx_t*)malloc(new_cnt * sizeof(idx_t));
        prime_list_cap = new_cnt;
    } else {
        prime_list = (idx_t*)realloc(prime_list, new_cnt * sizeof(idx_t));
        prime_list_cap = new_cnt;
    }

    memcpy(prime_list, new_primes, new_cnt * sizeof(idx_t));
    prime_list_cnt = new_cnt;
    free(new_primes);
}

/* Sum of divisors of n (computed in 64‑bit, enough for the tested range). */
static uint64_t sigma_of(idx_t n)
{
    uint64_t result = 1;
    idx_t tmp = n;

    for (size_t i = 0; i < prime_list_cnt; ++i) {
        uint32_t p = (uint32_t)prime_list[i];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)tmp)
            break;
        if (tmp % p == 0) {
            uint64_t term = 1;   /* (p^(e+1)-1)/(p-1) */
            uint64_t pow  = 1;   /* p^k */
            while (tmp % p == 0) {
                tmp /= p;
                pow *= (uint64_t)p;
                term += pow;
            }
            result *= term;
        }
    }

    if (tmp > 1) {           /* remaining prime factor > sqrt(n) */
        result *= ((uint64_t)tmp + 1ULL);
    }

    return result;
}

/* Required external function definition. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined for n = 0, but return 0 for safety */

    ensure_prime_list(n);                 /* make sure we have the n‑th prime */

    uint64_t prime_n = (uint64_t)prime_list[n - 1];
    uint64_t sigma_n = sigma_of(n);

    /* compute the modulus in 64‑bit, then cast to seq_t */
    uint64_t res64 = prime_n % sigma_n;
    return (seq_t)res64;
}
