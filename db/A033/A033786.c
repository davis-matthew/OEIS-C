/*
 * A033786  Product t2(q^d); d | 27, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in \prod_{d\mid 27} t2(q^d) * where
 *            t2(q) = \sum_{k\ge 0} q^{k(k+1)/2} * (i.e. the generating
 *            function of triangular numbers)
 * Range:     u8 n <= 1079, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of seq_t and idx_t provided by the build system. */
typedef seq_t seq_t;
typedef idx_t idx_t;

/* Static cache for computed values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static bool cache_initialized = false;

/* Compute all values up to max_n inclusive */
static void compute_cache(idx_t max_n)
{
    /* Allocate new cache */
    seq_t *new_cache = (seq_t*)malloc((max_n + 1) * sizeof(seq_t));
    if (!new_cache) return;           /* allocation failed – just abort */

    for (idx_t i = 0; i <= max_n; ++i) new_cache[i] = 0;

    /* Pre‑compute exponents for each divisor of 27
       Exponents are d * triangular_number(k) = d * k*(k+1)/2.   */
    idx_t exps1[512], exps3[512], exps9[512], exps27[512];
    idx_t s1 = 0, s3 = 0, s9 = 0, s27 = 0;

    for (idx_t k = 0;; ++k) {
        idx_t tri = k * (k + 1) / 2;
        if (tri > max_n) break;
        exps1[s1++] = tri;
    }
    for (idx_t k = 0;; ++k) {
        idx_t tri = k * (k + 1) / 2;
        idx_t exp = tri * 3;
        if (exp > max_n) break;
        exps3[s3++] = exp;
    }
    for (idx_t k = 0;; ++k) {
        idx_t tri = k * (k + 1) / 2;
        idx_t exp = tri * 9;
        if (exp > max_n) break;
        exps9[s9++] = exp;
    }
    for (idx_t k = 0;; ++k) {
        idx_t tri = k * (k + 1) / 2;
        idx_t exp = tri * 27;
        if (exp > max_n) break;
        exps27[s27++] = exp;
    }

    /* Enumerate all quadruples of exponents and count occurrences */
    for (idx_t i1 = 0; i1 < s1; ++i1) {
        idx_t e1 = exps1[i1];
        for (idx_t i3 = 0; i3 < s3; ++i3) {
            idx_t sum13 = e1 + exps3[i3];
            if (sum13 > max_n) continue;
            for (idx_t i9 = 0; i9 < s9; ++i9) {
                idx_t sum139 = sum13 + exps9[i9];
                if (sum139 > max_n) continue;
                for (idx_t i27 = 0; i27 < s27; ++i27) {
                    idx_t sum = sum139 + exps27[i27];
                    if (sum > max_n) continue;
                    /* Increment the coefficient for q^sum */
                    new_cache[sum] += 1;
                }
            }
        }
    }

    /* Replace the old cache */
    if (cache) free(cache);
    cache = new_cache;
    cache_size = max_n + 1;
    cache_initialized = true;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (!cache_initialized || n >= cache_size) {
        /* Compute at least up to n; on larger n recompute entirely */
        idx_t required = n + 1;
        if (required < 10000) required = 10000;   /* small default buffer */
        compute_cache(required);
    }
    return cache[n];
}
