/*
 * A143443  a(n) = n * A002321(n).
 * https://oeis.org/A143443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n * A002321(n) (Mertens function)
 * Range:     i8 n <= 42, i16 n <= 44, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Compute the sequence a(n) = n * M(n) where M(n) is the Mertens
   function (sum of μ(k) for k = 1..n).  We store a cumulative sum
   of μ(k) in a static array so that subsequent calls reuse the
   computation.  The code works for any idx_t and seq_t as required. */

static seq_t *mu_sum = NULL;       /* cumulative sums of μ(k)  */
static size_t mu_cap = 0;          /* allocated capacity of mu_sum */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; values for n = 0 are unused.  */
    if (n == 0) return 0;

    /* Ensure we have computed μ(k) up to n.  */
    idx_t need = n + 1;   /* we use indices 0..n */
    if ((size_t)need > mu_cap) {
        /* Allocate temporary arrays for the linear sieve. */
        int8_t *mu_vals = (int8_t*)malloc((size_t)need * sizeof(int8_t));
        uint8_t *is_comp = (uint8_t*)malloc((size_t)need * sizeof(uint8_t));
        if (!mu_vals || !is_comp) abort();

        memset(is_comp, 0, (size_t)need);
        mu_vals[0] = 0;           /* unused */
        mu_vals[1] = 1;

        /* Store primes discovered during the sieve.  */
        idx_t *primes = (idx_t*)malloc((size_t)need * sizeof(idx_t));
        if (!primes) abort();
        size_t primes_cnt = 0;

        for (idx_t i = 2; i < need; ++i) {
            if (!is_comp[i]) {
                primes[primes_cnt++] = i;
                mu_vals[i] = -1;
            }
            for (size_t j = 0; j < primes_cnt; ++j) {
                idx_t p = primes[j];
                idx_t ip = i * p;
                if (ip >= need) break;
                is_comp[ip] = 1;
                if (i % p == 0) {
                    mu_vals[ip] = 0;
                    break;
                } else {
                    mu_vals[ip] = -mu_vals[i];
                }
            }
        }
        free(primes);

        /* Reallocate the cumulative sum array to the new size. */
        seq_t *new_mu_sum = (seq_t*)realloc(mu_sum, (size_t)need * sizeof(seq_t));
        if (!new_mu_sum) abort();
        mu_sum = new_mu_sum;
        mu_cap = need;

        /* Compute the cumulative sums of μ(k). */
        seq_t sum = 0;
        mu_sum[0] = 0;
        for (idx_t i = 1; i < need; ++i) {
            sum += (seq_t)mu_vals[i];
            mu_sum[i] = sum;
        }

        free(mu_vals);
        free(is_comp);
    }

    /* Return a(n) = n * M(n).  */
    return (seq_t)n * mu_sum[n];
}
