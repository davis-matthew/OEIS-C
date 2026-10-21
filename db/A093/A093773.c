/*
 * A093773  a(n) is the smallest integer at which the value of the "truncated
 *          Mertens function" (= A088004) equals the n-th prime number.
 * https://oeis.org/A093773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest x such that A088004(x) = the n‑th prime
 *            (A093773). A088004(x) is the sum of μ(k) over all non‑prime k≤x
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static const idx_t LIMIT_X = 3000000;          /* upper bound for sieve */
static idx_t *primes = NULL;                  /* list of primes up to LIMIT_X */
static size_t primes_cnt = 0, primes_cap = 0;
static bool *is_prime = NULL;                 /* primality flag array */
static int8_t *mu = NULL;                     /* Möbius values */
static idx_t *min_x = NULL;                   /* min_x[H] = minimal x with H(x)=H */

static void init(void)
{
    /* Allocate basic arrays */
    idx_t max = LIMIT_X;

    is_prime = (bool *)malloc((max + 1) * sizeof(bool));
    mu = (int8_t *)malloc((max + 1) * sizeof(int8_t));
    min_x = (idx_t *)malloc((max + 1) * sizeof(idx_t));
    if (!is_prime || !mu || !min_x)
        exit(1);

    for (idx_t i = 0; i <= max; ++i) {
        is_prime[i] = true;
        mu[i] = 0;
        min_x[i] = 0;
    }
    is_prime[0] = is_prime[1] = false;
    mu[1] = 1;

    /* Allocate array for smallest prime factor used in linear sieve */
    idx_t *spf = (idx_t *)malloc((max + 1) * sizeof(idx_t));
    if (!spf)
        exit(1);
    for (idx_t i = 0; i <= max; ++i)
        spf[i] = 0;

    /* Linear sieve for primes and μ */
    primes_cap = max / 10 + 10;
    primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    if (!primes)
        exit(1);

    for (idx_t i = 2; i <= max; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = i;
            if (primes_cnt == primes_cap) {    /* grow array */
                primes_cap *= 2;
                primes = (idx_t *)realloc(primes,
                                            primes_cap * sizeof(idx_t));
                if (!primes)
                    exit(1);
            }
            primes[primes_cnt++] = i;
            mu[i] = -1;
            is_prime[i] = true;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            idx_t ip = i * p;
            if (ip > max)
                break;
            spf[ip] = p;
            is_prime[ip] = false;
            if (p == spf[i]) {                 /* square factor */
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }
    free(spf);

    /* Compute the truncated Mertens function H(x) iteratively
       and record the first x that attains each positive H value. */
    long long h = 0;  /* H(x) fits in 64‑bit for the chosen limit */
    for (idx_t i = 1; i <= max; ++i) {
        if (!is_prime[i])          /* non‑prime, add μ(i) */
            h += mu[i];

        if (h > 0 && h <= max && min_x[(idx_t)h] == 0)
            min_x[(idx_t)h] = i;
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init();
        initialized = true;
    }

    /* n is 1‑based; primes[] is 0‑based */
    idx_t prime_val = primes[n - 1];
    /* The minimal x with H(x)=prime_val was stored in min_x[prime_val] */
    return (seq_t)min_x[prime_val];
}
