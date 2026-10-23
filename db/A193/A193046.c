/*
 * A193046  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A193046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A193046.
   Recurrence: a(n) = 5*a(n‑1) – 9*a(n‑2) + 6*a(n‑3) + a(n‑4)
                 – 3*a(n‑5) + a(n‑6)
   Initial values: a(0)=1, a(1)=1, a(2)=17, a(3)=83, a(4)=275, a(5)=727
   (All computations are performed modulo the natural range of seq_t;
    intermediate results use 128‑bit arithmetic to avoid overflow.) */

#include <stdlib.h>   /* malloc, abort */
#include <stdint.h>   /* size_t */

static const seq_t base_values[6] = {1, 1, 17, 83, 275, 727};

static seq_t *cache = NULL;       /* cached sequence values      */
static size_t cache_length = 0;   /* number of terms currently cached */
static size_t cache_capacity = 0; /* allocated size of the cache   */

/* Small‑int multiplication using 128‑bit intermediate to avoid overflow. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    return (seq_t)((unsigned __int128)x * m);
}

/* Grow the cache to include terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_length) return;

    /* Resize cache if necessary. */
    if (need > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity << 1 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();
        if (cache) {
            /* copy old data */
            for (size_t i = 0; i < cache_length; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute remaining terms. */
    for (size_t i = cache_length; i < need; ++i) {
        if (i < 6) {
            cache[i] = base_values[i];
            continue;
        }
        seq_t pos = 0, neg = 0;
        pos += mul_small(cache[i-1], 5);
        neg += mul_small(cache[i-2], 9);
        pos += mul_small(cache[i-3], 6);
        pos += cache[i-4];
        neg += mul_small(cache[i-5], 3);
        pos += cache[i-6];
        /* It is guaranteed that pos >= neg, so no underflow. */
        cache[i] = pos - neg;
    }
    cache_length = need;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
