/*
 * A055037  Number of numbers <= n with an even number of prime factors
 *          (counted with multiplicity).
 * https://oeis.org/A055037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + L(n)) / 2, where L(n) = Σ_{k=1..n} λ(k) * and λ(k) =
 *            (-1)^{Ω(k)} is the Liouville function
 * Range:     u8 n <= 526, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache data */
static idx_t   max_n  = 0;
static idx_t * spf    = NULL;   /* smallest prime factor */
static int8_t * lambda = NULL; /* λ(i) stored as ±1 */
static seq_t * a_cache = NULL;

/* Extend caches up to at least n */
static void extend_cache(idx_t n)
{
    if (n <= max_n)
        return;

    idx_t new_n = n;

    /* Allocate new arrays */
    idx_t   *new_spf    = (idx_t*)malloc((new_n + 1) * sizeof(idx_t));
    int8_t  *new_lambda = (int8_t*)malloc((new_n + 1) * sizeof(int8_t));
    seq_t   *new_a      = (seq_t*)malloc((new_n + 1) * sizeof(seq_t));

    if (!new_spf || !new_lambda || !new_a)
        exit(EXIT_FAILURE);   /* memory exhausted */

    /* Initialise spf */
    new_spf[0] = 0;
    new_spf[1] = 1;
    for (idx_t i = 2; i <= new_n; ++i)
        new_spf[i] = 0;

    /* Simple sieve to fill spf */
    for (idx_t i = 2; i * i <= new_n; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            for (idx_t j = i * i; j <= new_n; j += i)
                if (new_spf[j] == 0)
                    new_spf[j] = i;
        }
    }
    for (idx_t i = 2; i <= new_n; ++i)
        if (new_spf[i] == 0)
            new_spf[i] = i;   /* remaining primes */

    /* Compute λ values */
    new_lambda[0] = 0;
    new_lambda[1] = 1;               /* λ(1) = 1 */
    for (idx_t i = 2; i <= new_n; ++i)
        new_lambda[i] = -new_lambda[i / new_spf[i]];

    /* Compute a(n) values */
    new_a[0] = 0;
    for (idx_t i = 1; i <= new_n; ++i) {
        seq_t add = (seq_t)((new_lambda[i] + 1) / 2); /* 1 if λ=1, else 0 */
        new_a[i] = new_a[i - 1] + add;
    }

    /* Replace old caches */
    free(spf);
    free(lambda);
    free(a_cache);
    spf    = new_spf;
    lambda = new_lambda;
    a_cache = new_a;
    max_n = new_n;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined offset, but keep zero */

    extend_cache(n);
    return a_cache[n];
}
