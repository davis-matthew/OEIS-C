/*
 * A175702  Convolution square of the Liouville sequence A008836.
 * https://oeis.org/A175702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} λ(k) * λ(n+1-k) (Liouville λ)
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Static cache for λ and smallest prime factor tables.  They grow only
   when a larger n is requested, independent of call order. */
static idx_t lambda_max = 0;      /* largest n for which λ is computed  */
static seq_t *lambda_cache = NULL;
static idx_t spf_max = 0;         /* largest n for which spf is computed   */
static idx_t *spf = NULL;

/* Ensure that spf[0..n] and λ[0..n] are available. */
static void ensure_tables(idx_t n)
{
    if (n <= lambda_max)   /* already up to date */ return;

    /* ---------- Compute SPF up to n ---------- */
    if (n > spf_max) {
        size_t new_spf_size = (size_t)n + 1;
        idx_t *new_spf = (idx_t *)realloc(spf, new_spf_size * sizeof(idx_t));
        if (!new_spf) abort();
        spf = new_spf;
        for (idx_t i = 0; i <= n; ++i) spf[i] = i;   /* init */

        idx_t i;
        for (i = 2; i * i <= n; ++i) {
            if (spf[i] == i) {                 /* i is prime */
                for (idx_t j = i * i; j <= n; j += i) {
                    if (spf[j] == j) spf[j] = i;
                }
            }
        }
        spf_max = n;
    }

    /* ---------- Compute λ up to n ---------- */
    size_t new_lambda_size = (size_t)n + 1;
    seq_t *new_lambda = (seq_t *)realloc(lambda_cache, new_lambda_size * sizeof(seq_t));
    if (!new_lambda) abort();
    lambda_cache = new_lambda;

    /* λ(1) = 1  (λ(0) is unused) */
    if (lambda_max < 1) lambda_cache[1] = 1;

    for (idx_t i = lambda_max + 1; i <= n; ++i) {
        /* λ(i) = -λ(i / spf[i])  because spf[i] is a prime factor. */
        lambda_cache[i] = -lambda_cache[i / spf[i]];
    }
    lambda_max = n;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset = 1, behaviour below 1 is undefined */

    ensure_tables(n);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += lambda_cache[k] * lambda_cache[n + 1 - k];
    }
    return sum;
}
