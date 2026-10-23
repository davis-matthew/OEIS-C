/*
 * A101552  C(n-3,3)+C(n-7,7)+...+C(n-(4*floor((n-4)/4)+3),4*floor((n-4)/4)+3).
 * https://oeis.org/A101552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k ≡ 3 (mod 4)} C(n-k, k) Recurrence:
 *            a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0, a(7)=1, a(8)=4 a(n) =
 *            4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) + a(n-8) for n ≥ 9
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 49, u64 n <= 95, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached values for any call order. */
static seq_t *cache          = NULL;
static idx_t  cache_size     = 0;   /* allocated number of slots */
static idx_t  computed_max   = 0;   /* highest index that has been computed */

/* First eight terms of the sequence. */
static const seq_t base[8] = {
    (seq_t)0, (seq_t)0, (seq_t)0,
    (seq_t)0, (seq_t)0, (seq_t)0,
    (seq_t)1, (seq_t)4
};

/* Allocate or extend the cache, and fill in any missing terms up to n. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Initialise the cache to hold indices 0..7. */
        cache_size = 8;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        if (!cache) exit(EXIT_FAILURE);
        for (idx_t i = 0; i < 8; ++i) cache[i] = base[i];
        computed_max = 7;
    }

    if (n >= cache_size) {
        /* Grow the array to cover index n. */
        size_t new_size = (size_t)n + 1U;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache   = tmp;
        cache_size = new_size;
    }

    if (n <= computed_max) return; /* all values up to n are already ready */

    /* Compute terms from computed_max+1 up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t t1 = 4 * cache[i-1];
        seq_t t2 = 6 * cache[i-2];
        seq_t t3 = 4 * cache[i-3];
        seq_t t4 = cache[i-4];
        seq_t t5 = cache[i-8];
        cache[i] = (t1 + t3 + t5) - (t2 + t4);  /* arithmetic is exact in seq_t */
    }
    computed_max = n;
}

/* Sequence entry point with external C linkage. */
seq_t a(idx_t n)
{
    if (n < 8) return base[n];
    ensure_cache(n);
    return cache[n];
}
