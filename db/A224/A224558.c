/*
 * A224558  Number of (n+2) X 9 0..1 matrices with each 3 X 3 subblock
 *          idempotent.
 * https://oeis.org/A224558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) -
 *            a(n-6) (valid for n>8, with a(1)=669, a(2)=492, a(3)=620,
 *            a(4)=749, a(5)=988, a(6)=1298, a(7)=1668, a(8)=2201)
 * Range:     u8 none, u16 n <= 18, u32 n <= 47, u64 n <= 105, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Initial terms as 64‑bit literals; cast to seq_t when used.
   Casting avoids compilation warnings for small seq_t types. */
static const unsigned long long init_raw[8] = {
    669ULL, 492ULL, 620ULL, 749ULL, 988ULL, 1298ULL, 1668ULL, 2201ULL
};

/* Compute the nth term from a cache that already contains terms
   up to k (k >= 8).  We use only additions and subtractions
   to avoid overflow on arbitrary‑precision seq_t types. */
static seq_t compute_term(const seq_t *cache, idx_t i)
{
    seq_t t1 = cache[i-1];
    seq_t t2 = cache[i-2];
    seq_t t3 = cache[i-3];
    seq_t t4 = cache[i-4];
    seq_t t5 = cache[i-5];
    seq_t t6 = cache[i-6];

    /* 4*t1 */
    seq_t p1 = t1 + t1;
    p1 += p1;                     /* 4*t1 */

    /* 6*t2 */
    seq_t p2 = t2 + t2;           /* 2*t2 */
    p2 += p2;                     /* 4*t2 */
    seq_t p2_6 = p2 + t2 + t2;    /* 6*t2 */

    /* 5*t3 */
    seq_t p3 = t3 + t3;           /* 2*t3 */
    p3 += p3 + t3;                /* 5*t3 */

    /* 4*t4 */
    seq_t p4 = t4 + t4;           /* 2*t4 */
    p4 += p4;                     /* 4*t4 */

    /* 3*t5 */
    seq_t p5 = t5 + t5;           /* 2*t5 */
    p5 += t5;                     /* 3*t5 */

    /* pos = 4*a(n-1) + 5*a(n-3) + 3*a(n-5) */
    seq_t pos = p1 + p3 + p5;

    /* negsum = 6*a(n-2) + 4*a(n-4) + a(n-6) */
    seq_t negsum = p2_6 + p4 + t6;

    /* result = pos - negsum   (assumed non‑negative for true sequence) */
    return pos - negsum;
}

/* Main sequence function.  Uses a static cache that is grown as needed
   to support any order of calls. */
seq_t a(idx_t n)
{
    /* Handle the first eight terms directly. */
    if (n <= 8) {
        return (seq_t)init_raw[n-1];
    }

    /* Static cache of previously computed terms (indices start at 1). */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;  /* highest index computed so far (>=8) */
    static size_t cache_size = 0; /* allocated size of cache (>= n+1) */

    /* Allocate or expand cache if necessary. */
    if (cache == NULL) {
        cache_size = (size_t)(n + 1);
        if (cache_size < 9) cache_size = 9;  /* at least for indices 1..8 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) {
            /* Out of memory – fatal for this sequence. */
            exit(EXIT_FAILURE);
        }
        /* Initialize the first eight entries. */
        for (idx_t i = 1; i <= 8; ++i) {
            cache[i] = (seq_t)init_raw[i-1];
        }
        max_cached = 8;
    } else if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] = compute_term(cache, i);
    }
    max_cached = n;

    return cache[n];
}
