/*
 * A208243  Number of ways to write 2n-1 = p+q, where p is a prime, and both q
 *          and q+2 are practical numbers (A005153).
 * https://oeis.org/A208243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write 2*n-1 = p + q where p is prime
 *            and both q and q+2 are practical numbers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The build supplies: seq_t, idx_t.  We use only idx_t for array
   indices and seq_t for the return value.                               */

/* ------------------------------------------------------------------ */
/*  Tables that are expanded on demand                                   */
/* ------------------------------------------------------------------ */
static idx_t   current_limit = 0;      /* highest number processed */
static bool   *is_prime      = NULL;   /* 0..current_limit */
static bool   *is_practical  = NULL;   /* 0..current_limit */
static uint64_t *prime_list = NULL;    /* array of primes up to current_limit */
static size_t   prime_count = 0;

/* ------------------------------------------------------------------ */
/*  Utility: resize tables to cover numbers up to 'new_limit'.        */
/* ------------------------------------------------------------------ */
static void ensure_limit(idx_t new_limit)
{
    if (new_limit <= current_limit) return;      /* nothing to do */

    /* allocate new tables */
    size_t sz = (size_t)new_limit + 1;          /* +1 for 0 index */

    bool *new_is_prime     = (bool *)calloc(sz, sizeof(bool));
    bool *new_is_practical = (bool *)calloc(sz, sizeof(bool));
    if (!new_is_prime || !new_is_practical) abort();

    /* sieve of Eratosthenes for primes */
    for (size_t i = 2; i < sz; ++i) new_is_prime[i] = true;
    for (size_t i = 2; i * i < sz; ++i)
        if (new_is_prime[i])
            for (size_t j = i * i; j < sz; j += i)
                new_is_prime[j] = false;

    /* build prime list */
    size_t pc = 0;
    for (size_t i = 2; i < sz; ++i) if (new_is_prime[i]) ++pc;
    uint64_t *new_primes = (uint64_t *)malloc(pc * sizeof(uint64_t));
    if (!new_primes) abort();
    for (size_t i = 2, k = 0; i < sz; ++i)
        if (new_is_prime[i]) new_primes[k++] = (uint64_t)i;

    /* helper: test whether n is practical by factorising with primes */
    for (size_t n = 1; n < sz; ++n) {
        if (n == 1) { new_is_practical[n] = true; continue; }

        uint64_t temp = n;
        uint64_t sigma_prev = 1;          /* product of sigma(p^e) for processed primes */
        bool ok = true;
        for (size_t pi = 0; pi < pc && (uint64_t)new_primes[pi] * new_primes[pi] <= temp; ++pi) {
            uint64_t p = new_primes[pi];
            if (temp % p == 0) {
                unsigned e = 0;
                while (temp % p == 0) { temp /= p; ++e; }
                if (p > sigma_prev + 1) { ok = false; break; }

                /* compute sigma(p^e) */
                uint64_t power = 1;
                for (unsigned k = 0; k <= e; ++k) power *= p;
                uint64_t sigma_pe = (power - 1) / (p - 1);
                sigma_prev *= sigma_pe;
            }
        }
        if (ok && temp > 1) {                 /* remaining prime factor with exponent 1 */
            uint64_t p = temp;
            if (p > sigma_prev + 1) ok = false;
            else {
                /* p has exponent 1, so sigma(p) = 1 + p */
                sigma_prev *= (1 + p);
            }
        }
        new_is_practical[n] = ok;
    }

    /* free old tables */
    free(is_prime);
    free(is_practical);
    free(prime_list);

    /* install new tables */
    is_prime      = new_is_prime;
    is_practical  = new_is_practical;
    prime_list    = new_primes;
    prime_count   = pc;
    current_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/*  Main function                                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined below offset */

    idx_t max_value = (idx_t)(2 * n - 1);

    ensure_limit(max_value);            /* guarantees tables up to max_value */

    seq_t count = 0;                     /* result counter */

    /* iterate over primes p <= max_value */
    for (size_t pi = 0; pi < prime_count; ++pi) {
        uint64_t p = prime_list[pi];
        if (p > (uint64_t)max_value) break;
        idx_t q = (idx_t)(max_value - p);
        if (q < 1) continue;
        if (is_practical[q] && is_practical[(size_t)(q + 2)])
            ++count;
    }

    return count;
}
