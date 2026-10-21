/*
 * A073578  a(n) = Sum_{k=1..n} mu(2*k).
 * https://oeis.org/A073578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=1..n} mu(2*k)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Static cache for prefix sums of μ on odd arguments.
   odd_prefix[i] = Σ_{odd k ≤ i} μ(k)  (type seq_t */
static seq_t *odd_prefix = NULL;
static idx_t cache_max = 0;

/* Build μ values up to n (inclusive) and fill odd_prefix. */
static void compute_upto(idx_t n)
{
    /* Linear sieve for μ up to n. */
    int8_t  *mu      = (int8_t  *)malloc((n + 1) * sizeof(int8_t));
    uint8_t *is_composite = (uint8_t *)calloc((n + 1), sizeof(uint8_t));
    idx_t   *primes = (idx_t   *)malloc((n + 1) * sizeof(idx_t));
    size_t   prime_cnt = 0;

    if (!mu || !is_composite || !primes) abort();

    mu[1] = 1;              /* μ(1) = 1 */

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_composite[i]) {          /* i is prime */
            primes[prime_cnt++] = i;
            mu[i] = -1;                 /* μ(prime) = -1 */
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = primes[j];
            idx_t t = i * p;
            if (t > n) break;
            is_composite[t] = 1;
            if (i % p == 0) {
                mu[t] = 0;             /* square factor */
                break;
            } else {
                mu[t] = -mu[i];        /* μ is multiplicative */
            }
        }
    }

    /* Allocate new odd_prefix array. */
    seq_t *new_prefix = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!new_prefix) abort();

    seq_t running = 0;
    new_prefix[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (i & 1) {                     /* i is odd */
            running += (seq_t)mu[i];
        }
        new_prefix[i] = running;
    }

    /* Replace old cache. */
    free(odd_prefix);
    odd_prefix = new_prefix;
    cache_max  = n;

    /* Free temporary arrays. */
    free(mu);
    free(is_composite);
    free(primes);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined (offset is 1), but safe */

    if (n > cache_max) compute_upto(n);

    /* a(n) = Sum_{k=1..n} μ(2*k)
       = - Σ_{odd k ≤ n} μ(k) */
    return -odd_prefix[n];
}
