/*
 * A017669  Numerator of sum of -3rd powers of divisors of n.
 * https://oeis.org/A017669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sum_{d|n} 1/d^3 = sigma_3(n) /
 *            gcd(sigma_3(n), n^3)
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1567, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *sigma = NULL;          /* array of sigma_3 values */
static idx_t  sigma_limit = 0;       /* maximum n for which sigma is computed */

static void compute_sigma_upto(idx_t n)
{
    if (sigma != NULL && n <= sigma_limit)
        return;                     /* already computed */

    /* Allocate new array big enough and zero it */
    seq_t *new_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!new_arr)
        return;                     /* allocation failure: caller will crash */
    for (idx_t i = 0; i <= n; ++i)
        new_arr[i] = 0;

    /* Sieve to compute sigma_3 for all numbers up to n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t cube = (seq_t)i * (seq_t)i * (seq_t)i;  /* i^3 */
        for (idx_t j = i; j <= n; j += i)
            new_arr[j] += cube;
    }

    /* Replace old array if any */
    if (sigma)
        free(sigma);
    sigma = new_arr;
    sigma_limit = n;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* OEIS offset is 1, but guard against 0 */
        return 0;

    compute_sigma_upto(n);

    seq_t sigma_n = sigma[n];
    seq_t n3       = (seq_t)n * (seq_t)n * (seq_t)n;

    seq_t g = gcd(sigma_n, n3);   /* integer gcd for seq_t */
    return sigma_n / g;           /* reduced numerator */
}
