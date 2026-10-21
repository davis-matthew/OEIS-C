/*
 * A063849  Numbers k such that mu(k) + mu(k+1) + mu(k+2) = 1.
 * https://oeis.org/A063849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation precomputes the Möbius function μ(n) via a linear
 *  sieve up to a fixed limit.  It then collects, in order, all integers k
 *  for which μ(k)+μ(k+1)+μ(k+2)=1.  The terms are stored in a static
 *  array and generated lazily as the function a(idx_t n) is called.
 *  The limit 2 000 000 is sufficient to produce at least the first
 *  10 000 terms (the density of valid k is roughly 0.15). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Types seq_t and idx_t are supplied by the build system. */

static const size_t LIMIT = 2000000U;   /* enough for n up to about 10k */

static int8_t *mu = NULL;          /* Möbius values, 1‑based */
static unsigned char *is_prime = NULL;   /* sieve flag */
static size_t *primes = NULL;      /* list of primes */
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static seq_t *terms = NULL;      /* found k‑values */
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t next_k = 1;         /* next integer to test */

static bool initialized = false;

/* ------------------------------------------------------------------ */
/*  Compute μ[1..LIMIT] with a linear sieve.                          */
/* ------------------------------------------------------------------ */
static void init_mu(void)
{
    mu = (int8_t *)malloc((LIMIT + 3) * sizeof(int8_t));
    is_prime = (unsigned char *)malloc((LIMIT + 3) * sizeof(unsigned char));
    if (!mu || !is_prime) exit(1);

    mu[1] = 1;
    for (size_t i = 1; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    primes_cap = 100000;          /* initial capacity */
    primes = (size_t *)malloc(primes_cap * sizeof(size_t));
    if (!primes) exit(1);

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                size_t *tmp = (size_t *)realloc(primes, primes_cap * sizeof(size_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[primes_cnt++] = i;
            mu[i] = -1;
        }

        for (size_t pj = 0; pj < primes_cnt; ++pj) {
            size_t p = primes[pj];
            size_t ip = i * p;
            if (ip > LIMIT) break;
            is_prime[ip] = 0;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Ensure that we have at least 'n' terms stored.                    */
/* ------------------------------------------------------------------ */
static void ensure_terms(idx_t n)
{
    while (terms_len < (size_t)n) {
        if ((size_t)next_k + 2 > LIMIT) {
            /* Out of pre‑computed range; not expected for requested n. */
            exit(1);
        }
        int sum = mu[next_k] + mu[next_k + 1] + mu[next_k + 2];
        if (sum == 1) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* ------------------------------------------------------------------ */
/*  Public interface                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_mu();
        initialized = true;
    }
    if (n == 0) return 0;                 /* undefined for n<1 */
    ensure_terms(n);
    return terms[n - 1];
}
