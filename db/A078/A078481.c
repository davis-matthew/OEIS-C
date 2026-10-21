/*
 * A078481  Expansion of (1 - x - x^2 - sqrt(1 - 2*x - 5*x^2 - 2*x^3 + x^4)) /
 *          (2*x + 2*x^2).
 * https://oeis.org/A078481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence: (n+1)*a(n) = (n-2)*a(n-1) + (7*n-11)*a(n-2) +
 *            (7*n-17)*a(n-3) + (n-2)*a(n-4) - (n-5)*a(n-5) Initial values:
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=3, a(4)=7
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 40, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamically expanded cache         */
static idx_t cache_len = 0;      /* number of terms stored so far      */

/* Forward declaration of helper to compute terms up to n */
static void build_cache_to(idx_t n);

seq_t a(idx_t n)
{
    /* Ensure cache contains all terms up to n */
    if (n >= cache_len)
        build_cache_to(n);
    return cache[n];
}

/* Helper: expand cache and compute missing terms */
static void build_cache_to(idx_t n)
{
    /* Allocate or enlarge cache */
    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)
            abort();  /* worst‑case; in real code handle error */
        cache = tmp;
    }

    /* Compute terms that are missing */
    for (idx_t k = cache_len; k <= n; ++k) {
        if (k == 0) {
            cache[0] = 0;
        } else if (k == 1) {
            cache[1] = 1;
        } else if (k == 2) {
            cache[2] = 1;
        } else if (k == 3) {
            cache[3] = 3;
        } else if (k == 4) {
            cache[4] = 7;
        } else {                /* k >= 5 : use the recurrence */
#ifdef __cplusplus
            /* 64‑bit and arbitrary‑precision variant – rely on seq_t operators */
            seq_t term1 = cache[k-1];
            term1 *= (seq_t)(k-2);

            seq_t term2 = cache[k-2];
            term2 *= (seq_t)(7*k-11);

            seq_t term3 = cache[k-3];
            term3 *= (seq_t)(7*k-17);

            seq_t term4 = cache[k-4];
            term4 *= (seq_t)(k-2);

            seq_t term5 = cache[k-5];
            term5 *= (seq_t)(k-5);

            seq_t sum = 0;
            sum += term1;
            sum += term2;
            sum += term3;
            sum += term4;
            sum -= term5;

            cache[k] = sum / (seq_t)(k+1);
#else
            /* C implementation – use 128‑bit intermediate to avoid overflow */
            unsigned __int128 term1 = (unsigned __int128)(k-2) * cache[k-1];
            unsigned __int128 term2 = (unsigned __int128)(7*k-11) * cache[k-2];
            unsigned __int128 term3 = (unsigned __int128)(7*k-17) * cache[k-3];
            unsigned __int128 term4 = (unsigned __int128)(k-2) * cache[k-4];
            unsigned __int128 term5 = (unsigned __int128)(k-5) * cache[k-5];

            unsigned __int128 sum = term1 + term2 + term3 + term4 - term5;

            cache[k] = (seq_t)(sum / (k+1));
#endif
        }
    }
    cache_len = n + 1;
}
