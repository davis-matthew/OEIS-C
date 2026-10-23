/*
 * A130002  Alternating sum along antidiagonals of the array of A129952 and
 *          its iterated differences.
 * https://oeis.org/A130002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 8*a(n-3) + 7*a(n-4) + 4*a(n-5) -
 *            4*a(n-6), for n > 6; Initial values: a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=10, a(5)=23, a(6)=60
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t max_computed = 0;

/* Ensure cache has room for indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1) * 2;  /* double for amortized growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* If memory allocation fails, terminate immediately. */
            exit(1);
        }
        cache = tmp;
        cache_size = new_size;
    }
}

/* Initialize the first seven terms if they are not yet stored. */
static void
init_base(void)
{
    if (max_computed >= 6)
        return;

    ensure_cache(6);
    static const seq_t init[7] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3,
        (seq_t)10, (seq_t)23, (seq_t)60
    };
    for (idx_t i = 0; i <= 6; ++i)
        cache[i] = init[i];
    max_computed = 6;
}

/* Main sequence function as required by the benchmark. */
seq_t
a(idx_t n)
{
    /* Ensure base terms are initialized. */
    init_base();

    if (n <= max_computed) {
        /* The term is already known. */
        return cache[n];
    }

    /* Compute missing terms up to n. */
    ensure_cache(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];
        seq_t t6 = cache[i - 6];

        cache[i] = 4 * t1
                 - 2 * t2
                 - 8 * t3
                 + 7 * t4
                 + 4 * t5
                 - 4 * t6;
    }

    max_computed = n;
    return cache[n];
}
