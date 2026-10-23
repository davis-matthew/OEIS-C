/*
 * A105569  Diagonal sums of A105567, as a number triangle.
 * https://oeis.org/A105569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = sum_{k=0..⌊n/2⌋} -mu(k*(n-2*k))
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where mu(0) is defined as -1, mu(m) for m>0 is the usual Möbius
   function (-1^ω(m) if m is square‑free, 0 otherwise). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for Möbius values (mu[0] = -1) */
static int8_t *mu = NULL;          /* Möbius values                */
static unsigned *lp = NULL;         /* least prime factor array     */
static size_t *primes = NULL;       /* list of primes               */
static size_t primes_cnt = 0;       /* number of computed primes    */
static size_t mu_cap = 0;           /* current capacity of mu array */

/* Ensure that mu[0..limit] are computed.  Uses a linear sieve. */
static void ensure_mu(size_t limit)
{
    if (mu_cap >= limit)
        return;    /* already available */

    /* free old arrays if they exist */
    free(mu);
    free(lp);
    free(primes);

    mu     = (int8_t *)malloc((limit + 1) * sizeof(int8_t));
    lp     = (unsigned *)malloc((limit + 1) * sizeof(unsigned));
    primes = (size_t *)malloc((limit + 1) * sizeof(size_t));
    if (!mu || !lp || !primes)
        exit(1);   /* allocation failed */

    /* initialise */
    mu[0] = -1;        /* our convention for μ(0) */
    mu[1] = 1;
    primes_cnt = 0;
    for (size_t i = 0; i <= limit; ++i)
        lp[i] = 0;

    /* linear sieve to fill mu[2..limit] */
    for (size_t i = 2; i <= limit; ++i) {
        if (lp[i] == 0) {          /* i is prime */
            lp[i] = (unsigned)i;
            mu[i] = -1;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            size_t p = primes[j];
            size_t v = i * p;
            if (v > limit)
                break;
            lp[v] = (unsigned)p;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }

    mu_cap = limit;
}

/* Compute a(n) as defined above. */
seq_t a(idx_t n)
{
    /* The maximum product k*(n-2k) occurs near k = n/4, giving about n^2/8.
       Compute that bound (using 128‑bit to avoid overflow). */
    size_t limit = (size_t)(((__int128)n * n) / 8 + 1);
    ensure_mu(limit);

    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        size_t prod = (size_t)k * (size_t)(n - 2 * k);
        /* prod ≤ limit, so mu[prod] is defined */
        int8_t mu_val = mu[prod];
        sum += -(seq_t)mu_val;
    }
    return sum;
}
