/*
 * A054404  Number of daughters to wait before picking in sultan's dowry
 *          problem with n daughters.
 * https://oeis.org/A054404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = argmax_{1≤r≤n-1} (r/n)*(1/r+1/(r+1)+...+1/(n-1)) The
 *            function uses a simple O(n²) worst‑case precomputation that is
 *            efficient for n≤10 000 and guarantees exact integer answers for
 *            all required n
 * Range:     u8 n <= 695, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t  *cache   = NULL;   /* a(n) cached values (1-based) */
static double *H       = NULL;   /* harmonic numbers H[i] = sum_{k=1}^i 1/k (H[0]=0) */
static idx_t  cache_cap = 0;     /* number of cached elements (last computed n) */

/* Ensure the cache and harmonic array contain values up to n. */
static void extend_cache(idx_t n) {
    if (n <= cache_cap) return;               /* already available */

    /* Allocate/resize cache array */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    } else {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    }

    /* Allocate/resize harmonic array */
    if (!H) {
        H = (double *)malloc(sizeof(double) * (n + 1));
    } else {
        H = (double *)realloc(H, sizeof(double) * (n + 1));
    }

    /* Initialize omitted indices */
    if (cache_cap == 0) {
        H[0] = 0.0;
    }

    /* Compute harmonic numbers and a(n) for new indices */
    for (idx_t cur = cache_cap + 1; cur <= n; ++cur) {
        H[cur] = H[cur - 1] + 1.0 / (double)cur;
    }

    /* Compute a(cur) for cur = old_cap+1 .. n */
    for (idx_t cur = (cache_cap == 0 ? 1 : cache_cap + 1); cur <= n; ++cur) {
        if (cur == 1) {
            cache[cur] = (seq_t)0;               /* a(1) = 0 by definition */
            continue;
        }
        double best_val = -1.0;
        seq_t best_r = 1;
        double Hn1 = H[cur - 1];
        for (idx_t r = 1; r <= cur - 1; ++r) {
            double val = (double)r * (Hn1 - H[r - 1]);   /* r * (H[n-1] - H[r-1]) */
            if (val > best_val) {
                best_val = val;
                best_r   = (seq_t)r;
            }
        }
        cache[cur] = best_r;
    }

    cache_cap = n;
}

/* Public interface required by the build system */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;      /* undefined for n<1, return 0 as safe value */
    extend_cache(n);
    return cache[n];
}
