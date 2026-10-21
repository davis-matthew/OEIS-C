/*
 * A066934  Numbers k such that phi(k)==1 (mod bigomega(k)) where
 *          phi(n)=A000010(n) is the Euler totient function and
 *          bigomega(n)=A001222(n) is the number of prime divisors of n
 *          (counted with multiplicity).
 * https://oeis.org/A066934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that phi(k) == 1 (mod bigomega(k))
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PRIMES_LIMIT 5000000   /* should cover k needed for n≈10000 */

/* --- Prime sieve -------------------------------------------------- */
static uint32_t *primes = NULL;
static size_t primes_count = 0;
static size_t primes_cap   = 0;

static void init_primes(void)
{
    if (primes != NULL) return;                /* already done */

    /* Allocate array for the sieve (1 byte per flag). */
    unsigned char *is_prime = (unsigned char *)malloc(PRIMES_LIMIT + 1);
    if (!is_prime) exit(1);
    memset(is_prime, 1, PRIMES_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    /* Simple Eratosthenes sieve */
    for (size_t i = 2; i * i <= PRIMES_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIMES_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Reserve initial capacity for prime list */
    primes_cap = 100000;                       /* enough for many primes */
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(1);

    /* Fill the primes array */
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i) {
        if (is_prime[i]) {
            if (primes_count == primes_cap) {
                primes_cap *= 2;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[primes_count++] = (uint32_t)i;
        }
    }
    free(is_prime);
}

/* --- Cache of found sequence terms -------------------------------- */
static seq_t *terms   = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t next_k = 2;          /* next integer to test */

static void ensure_terms(size_t need)
{
    if (terms_len >= need) return;

    if (terms == NULL) {
        terms_cap = (need > 8) ? need : 8;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) exit(1);
    }

    while (terms_len < need) {
        init_primes();                     /* ensure prime list is ready */

        idx_t k    = next_k;
        idx_t temp = k;
        seq_t phi   = (seq_t)k;            /* start with n */
        seq_t omega = 0;

        /* Factorise k using the precomputed prime list */
        for (size_t pi = 0; pi < primes_count &&
                            (idx_t)primes[pi] * (idx_t)primes[pi] <= temp; ++pi) {
            uint32_t p = primes[pi];
            if (temp % p == 0) {
                int cnt = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++cnt;
                }
                phi   = phi / (seq_t)p * ((seq_t)p - 1);
                omega += cnt;
            }
        }
        if (temp > 1) {
            /* remaining factor is prime */
            phi   = phi / (seq_t)temp * ((seq_t)temp - 1);
            omega += 1;
        }

        if (omega > 0 && (phi % omega) == 1) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!terms) exit(1);
            }
            terms[terms_len++] = (seq_t)k;
        }
        ++next_k;
    }
}

/* --- Main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined offset, but harmless */
    ensure_terms((size_t)n);
    return terms[n - 1];
}
