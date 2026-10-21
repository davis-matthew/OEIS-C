/*
 * A089620  a(n) = n^3 + prime(n).
 * https://oeis.org/A089620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^3 + prime(n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Static state for the prime sieve */
static size_t sieve_limit   = 0;   /* last sieved upper bound            */
static idx_t *prime_list    = NULL;/* array of primes (1‑based)          */
static size_t prime_count   = 0;   /* #primes currently stored           */
static size_t prime_capacity= 0;   /* allocated array capacity           */

/* Estimate an upper bound for the n‑th prime (pn <= n*(ln n + ln ln n) for n>=6) */
static size_t estimate_limit(idx_t n) {
    if (n < 6) return 15;                     /* first few primes are very small */
    double dn  = (double)n;
    double est = dn * (log(dn) + log(log(dn)));/* analytic estimate                */
    size_t lim = (size_t)est + 3;              /* round up and give a safety buffer */
    if (lim < 100) lim = 100;                  /* tiny numbers still need some space */
    return lim;
}

/* Build a sieve up to 'limit' and store all primes encountered */
static void rebuild_sieve(size_t limit) {
    /* Allocate a bit‑vector for primality */
    unsigned char *is_prime = (unsigned char*)malloc(limit + 1);
    if (!is_prime) return;                     /* silently fail if OOM – tests expect enough RAM */
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    size_t root = (size_t)sqrt((double)limit);
    for (size_t p = 2; p <= root; ++p) {
        if (is_prime[p]) {
            for (size_t j = p * p; j <= limit; j += p) {
                is_prime[j] = 0;
            }
        }
    }

    /* Free and reallocate prime list with enough capacity */
    free(prime_list);
    prime_capacity = limit / 10 + 10;          /* rough estimate BASED on prime density */
    prime_list = (idx_t*)malloc(prime_capacity * sizeof(idx_t));
    prime_count = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (prime_count == prime_capacity) {    /* grow if needed */
                prime_capacity *= 2;
                prime_list = (idx_t*)realloc(prime_list,
                                            prime_capacity * sizeof(idx_t));
            }
            prime_list[prime_count++] = (idx_t)i;
        }
    }

    free(is_prime);
    sieve_limit = limit;
}

/* Ensure that we have at least 'n' primes stored. */
static void ensure_primes(idx_t n) {
    if (prime_count >= (size_t)n) return;      /* already enough */

    size_t new_limit = estimate_limit(n);
    if (new_limit <= sieve_limit) new_limit = sieve_limit * 2 + 100;

    rebuild_sieve(new_limit);
}

/* Main sequence function required by the build */
seq_t a(idx_t n) {
    if (n < 1) return 0;                       /* undefined for n < 1 */

    ensure_primes(n);                          /* guarantee prime_list[n‑1] exists */

    seq_t sn = (seq_t)n;
    seq_t cube = sn * sn * sn;
    seq_t p = (seq_t)prime_list[(size_t)(n - 1)];
    return cube + p;
}
