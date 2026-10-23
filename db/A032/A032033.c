/*
 * A032033  Stirling transform of A032031.
 * https://oeis.org/A032033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032033 (Stirling transform of A032031)
 * Formula: a(0) = 1
 * For n > 0:   a(n) = 3 * Σ_{k=0}^{n-1} binomial(n,k) * a(k)
 * Efficient computation is done with Stirling numbers of the second kind:
 *   a(n) = Σ_{k=0}^{n} S(n,k) * (3^k) * k!
 * The following implementation iteratively builds these values in O(n^2) time
 * and uses only integer arithmetic on the type seq_t.
 * The result is cached so that out‑of‑order calls are handled in O(n) per
 * maximal requested n. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* lazily allocated caches */
    static seq_t *cache      = NULL;   /* stores a values */
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* buffers for Stirling recurrence: two interchangeable arrays */
    static seq_t *val[2]        = {NULL, NULL};
    static size_t val_cap[2]    = {0, 0};
    static size_t val_size     = 0;     /* size of current val array = current n+1 */
    static int cur              = 0;    /* which of val[0]/val[1] is active */

    /* Initialise caches on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        cache[0] = 1;          /* a(0) = 1 */
        cache_len = 1;

        /* initial Stirling vector for n = 0: S(0,0)=1 */
        val_cap[0] = 1;
        val[0] = (seq_t*)calloc(val_cap[0], sizeof(seq_t));
        val[0][0] = 1;
        val_size = 1;
        cur = 0;
    }

    /* Grow cache if we need more terms */
    while (n >= cache_len) {
        size_t i = cache_len;   /* compute term a(i) */

        /* step: compute Stirling values for n = i using current val (n-1) */
        size_t next = 1 - cur;
        size_t need = i + 1;     /* size for new Stirling vector (n = i) */

        /* ensure capacity for target array */
        if (val_cap[next] < need) {
            size_t newc = val_cap[next] ? val_cap[next] * 2 : 1;
            while (newc < need) newc <<= 1;
            val[next] = (seq_t*)realloc(val[next], newc * sizeof(seq_t));
            val_cap[next] = newc;
        }

        val[next][0] = 0;  /* S(i,0)=0 for i>0 */

        /* compute new Stirling scaled by 3^k * k! */
        for (size_t k = 1; k <= i; ++k) {
            seq_t term1 = (k < val_size) ? val[cur][k] : 0;
            seq_t term2 = val[cur][k-1];
            /* new[k] = k * term1 + k * 3 * term2 */
            val[next][k] = (seq_t)k * term1 + ((seq_t)k * (seq_t)3 * term2);
        }

        /* sum of Stirling terms gives a(i) */
        seq_t sum = 0;
        for (size_t k = 0; k <= i; ++k) {
            sum += val[next][k];
        }

        /* store a(i) in cache */
        if (cache_cap <= i) {
            size_t newc = cache_cap ? cache_cap * 2 : 1;
            while (newc <= i) newc <<= 1;
            cache = (seq_t*)realloc(cache, newc * sizeof(seq_t));
            cache_cap = newc;
        }
        cache[i] = sum;

        /* prepare for next iteration */
        cur = next;
        val_size = need;
        cache_len = i + 1;
    }

    return cache[n];
}
