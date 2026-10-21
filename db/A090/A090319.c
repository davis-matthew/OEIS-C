/*
 * A090319  Fifth column (k=4) of triangle A084938.
 * https://oeis.org/A090319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{a+b+c+d=n} a!*b!*c!*d! (4‑fold convolution of
 *            factorials)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

#define MAX_N 10000   /* sufficient for the test suite */

static seq_t fact[MAX_N + 1];
static seq_t conv2[MAX_N + 1];
static seq_t a_cache[MAX_N + 1];
static idx_t  cache_max = 0;          /* highest index whose value is computed */

/* Ensure the factorial, two‑fold and four‑fold convolution tables are up to 'n' */
static void ensure_tables(idx_t n)
{
    if (n > MAX_N) return;   /* caller guarantees n <= MAX_N */

    /*  Initialization for the very first call */
    if (cache_max == 0 && n >= 0) {
        fact[0]   = (seq_t)1;
        conv2[0]  = fact[0] * fact[0];   /* =1 */
        a_cache[0] = conv2[0];           /* =1 */
        cache_max = 0;
    }

    /* Compute factorials beyond the current cache */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* Two‑fold convolution of factorials: conv2[i] = Σ_{j=0..i} fact[j]*fact[i‑j] */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 0; j <= i; ++j) {
            sum += fact[j] * fact[i - j];
        }
        conv2[i] = sum;
    }

    /* Four‑fold convolution: a_cache[i] = Σ_{k=0..i} conv2[k]*conv2[i‑k] */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; ++k) {
            sum += conv2[k] * conv2[i - k];
        }
        a_cache[i] = sum;
    }

    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n > MAX_N) { /* guard against out‑of‑range requests */
        return (seq_t)0;
    }
    ensure_tables(n);
    return a_cache[n];
}
