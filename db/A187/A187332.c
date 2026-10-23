/*
 * A187332  Complement of A187331.
 * https://oeis.org/A187332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-6) - a(n-7) for n > 7 Initial values:
 *            a(1)=2, a(2)=3, a(3)=5, a(4)=7, a(5)=8, a(6)=9, a(7)=12
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;      /* dynamic array holding a(0)...a(n) */
static idx_t cache_size = 0;     /* current allocated size */
static idx_t computed = 0;       /* highest index that has been computed */

/* Initial values (index 0 unused) */
static const seq_t init_vals[8] = {0, 2, 3, 5, 7, 8, 9, 12};

static void ensure_cache(idx_t n)
{
    /* Ensure cache has space for indices 0..n */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    } else if (cache_size <= n) {
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }
    /* Newly allocated elements are uninitialized; they will be set later */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined but safe guard */

    /* Make sure we can store up to 'n' */
    ensure_cache(n);

    /* Initialize first 7 entries if not already done */
    if (computed < 7) {
        idx_t i;
        for (i = 1; i <= 7 && i <= n; ++i) {
            if (i > computed) {
                cache[i] = init_vals[i];
                computed = i;
            }
        }
    }

    /* Compute any missing values up to 'n' */
    if (computed < n) {
        idx_t i;
        for (i = computed + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 7) {
                val = init_vals[i];      /* should already be set, but keep safe */
            } else {
                val = cache[i - 1] + cache[i - 6] - cache[i - 7];
            }
            cache[i] = val;
        }
        computed = n;
    }

    return cache[n];
}
