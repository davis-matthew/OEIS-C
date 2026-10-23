/*
 * A100198  Let f(0) = -1, f(n) = Moebius(n) = A008683(n) for n>0. Sequence
 *          gives partial sums a(n) = Sum_{ 0 <= i <= n} f(i).
 * https://oeis.org/A100198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=0..n} f(i) with f(0) = -1, f(i) = μ(i) for i>0
 *            where μ is the Möbius function (seq_t and idx_t supplied by the
 *            build)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of the cumulative sums */
    static idx_t max_n = 0;        /* largest n that has been computed */
    static seq_t *cum = NULL;     /* array a(0) .. a(max_n) */

    /* Handle the n==0 case separately to avoid out‑of‑bounds accesses. */
    if (n == 0) {
        if (cum == NULL) {
            cum = (seq_t*)malloc(sizeof(seq_t));
            if (!cum) return 0;      /* allocation failure */
            cum[0] = (seq_t)(-1);
            max_n = 0;
        }
        return cum[0];
    }

    /* If we have not computed up to n yet, do the sieve and accumulate. */
    if (n > max_n) {
        idx_t new_cap = n + 1U;                     /* size of arrays: indices 0..n */
        /* Resize the cumulative array. */
        seq_t *new_cum = (seq_t*)realloc(cum, new_cap * sizeof(seq_t));
        if (!new_cum) return 0;
        cum = new_cum;

        /* Temporary arrays for the linear sieve. */
        int32_t *mu      = (int32_t*)malloc(new_cap * sizeof(int32_t));
        uint8_t *isComp  = (uint8_t*)malloc(new_cap * sizeof(uint8_t));
        uint32_t *primes = (uint32_t*)malloc((new_cap/10U + 10U) * sizeof(uint32_t));
        if (!mu || !isComp || !primes) {
            free(mu); free(isComp); free(primes);
            return 0;
        }

        /* Initialise; indices 0 and 1 are not composites. */
        for (idx_t i = 0; i <= n; ++i) isComp[i] = 0;
        mu[1] = 1;

        /* Linear sieve to compute μ(2..n). */
        size_t primes_cnt = 0;
        size_t primes_cap = new_cap/10U + 10U;
        for (idx_t i = 2; i <= n; ++i) {
            if (!isComp[i]) {            /* i is prime */
                if (primes_cnt == primes_cap) {
                    primes_cap *= 2;
                    uint32_t *tmp = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                    if (!tmp) { free(mu); free(isComp); free(primes); return 0; }
                    primes = tmp;
                }
                primes[primes_cnt++] = (uint32_t)i;
                mu[i] = -1;              /* Möbius of a prime is -1 */
            }
            for (size_t j = 0; j < primes_cnt; ++j) {
                uint32_t p = primes[j];
                idx_t v = i * (idx_t)p;
                if (v > n) break;
                isComp[v] = 1;
                if (i % p == 0) {        /* p divides i */
                    mu[v] = 0;           /* squared prime factor -> 0 */
                    break;
                } else {
                    mu[v] = -mu[i];     /* otherwise μ(v) = -μ(i) */
                }
            }
        }

        /* Build the cumulative sums a(0)..a(n). */
        cum[0] = (seq_t)(-1);          /* f(0) = -1 */
        for (idx_t i = 1; i <= n; ++i) {
            cum[i] = cum[i-1] + (seq_t)mu[i];
        }

        /* Free the temporary arrays. */
        free(mu);
        free(isComp);
        free(primes);

        max_n = n;
    }

    return cum[n];
}
