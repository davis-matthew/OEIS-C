/*
 * A033757  Expansion of Product_{d | 45} theta_3(q^d).
 * https://oeis.org/A033757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 692, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033757: expansion of ∏_{d|45} θ₃(q^d)
   where θ₃(q) = Σ_{m∈ℤ} q^{m²}.  The coefficient a(n) counts the
   number of representations of n as
   a + 3b + 5c + 9d + 15e + 45f with a,b,c,d,e,f∈ℤ.  */
/* The code computes all coefficients up to the largest n requested
   by any call, using a simple convolution of the six sparse series
   corresponding to the six divisors of 45. */

#include <stdlib.h>
#include <string.h>

/* Helper to convert idx_t to seq_t for multiplication without overflow */
static inline seq_t to_seq(idx_t x) { return (seq_t)x; }

/* Static cache of coefficients */
static seq_t *cache = NULL;          /* array of coefficients */
static idx_t cache_size = 0;         /* number of valid indices (cache_size-1 = last computed) */

/* Compute coefficients up to 'limit' and store in cache */
static void compute_up_to(idx_t limit)
{
    /* Allocate new array of size limit+1 */
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)(limit + 1));
    if (!new_cache) { /* fallback to zero-return if allocation fails */
        return;
    }
    memset(new_cache, 0, sizeof(seq_t) * (size_t)(limit + 1));
    new_cache[0] = 1;                      /* start with empty product */

    /* Divisors of 45: 1,3,5,9,15,45 */
    const idx_t divisors[6] = {1, 3, 5, 9, 15, 45};

    /* Temporary array for convolution */
    seq_t *temp = (seq_t *)malloc(sizeof(seq_t) * (size_t)(limit + 1));
    if (!temp) { free(new_cache); return; }
    memset(temp, 0, sizeof(seq_t) * (size_t)(limit + 1));

    for (int di = 0; di < 6; ++di) {
        idx_t d = divisors[di];
        /* reset temp to zero */
        memset(temp, 0, sizeof(seq_t) * (size_t)(limit + 1));

        /* For each possible m such that d*m^2 <= limit */
        for (idx_t m = 0; m * m <= limit / d; ++m) {
            idx_t k = d * m * m;
            seq_t coeff = (m == 0 ? (seq_t)1 : (seq_t)2);

            /* Add contribution of this term to temp */
            for (idx_t i = k; i <= limit; ++i) {
                temp[i] += new_cache[i - k] * coeff;
            }
        }
        /* Swap new_cache and temp for next iteration */
        seq_t *swap = new_cache;
        new_cache = temp;
        temp = swap;
    }

    /* new_cache now holds final coefficients up to limit */
    /* Free old cache if present */
    free(cache);
    cache = new_cache;
    cache_size = limit + 1;  /* valid indices are 0..limit */

    free(temp);
}

seq_t a(idx_t n)
{
    /* Lazily compute coefficients up to n */
    if (n >= cache_size) {
        compute_up_to(n);
        /* if still not computed, return 0 as fallback */
        if (n >= cache_size) return (seq_t)0;
    }
    return cache[n];
}
