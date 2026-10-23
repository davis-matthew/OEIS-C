/*
 * A134035  The 4 X 4 Fibonacci/ anti-Fibonacci game switched modulo 2 with
 *          its dual: MA1={{0,1},{1,1}};MB1={{0,1}{1,3}};
 *          MA2={{0,1},1,3}};MB2={{1,0},{1,1}}; the game has two
 *          characteristic polynomials: (-3 + 5 x - 3 x^3 + x^4, -1 + x + 2
 *          x^2 - 3 x^3 + x^4}.
 * https://oeis.org/A134035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134035
   Recurrence:
   a(n) =  a(n-1) + 6*a(n-2) - 6*a(n-3) - 8*a(n-4) +  8*a(n-5)
          + 3*a(n-6) - 3*a(n-7)
   Initial values:
   a(1)=2, a(2)=4, a(3)=8, a(4)=13, a(5)=21, a(6)=39, a(7)=64
   The implementation uses a static cache and lazy evaluation.            */
#include <stddef.h>
#include <stdlib.h>

/* Initial terms (a(1) … a(7)) */
static const seq_t init[7] = { (seq_t)2, (seq_t)4, (seq_t)8,
                               (seq_t)13, (seq_t)21, (seq_t)39,
                               (seq_t)64 };

/* Cache: cache[i] holds a(i) for i>=1; cache[0] unused. */
static seq_t *cache   = NULL;   /* allocated on first use          */
static size_t cache_cap = 0;    /* capacity in seq_t elements       */
static size_t cache_len = 0;    /* next free index (1‑based)        */

/* Ensure that cache has capacity for indices up to n (inclusive) */
static void ensure_cache_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;                /* because cache[0] unused */
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;

    /* Use an explicit cast so the code compiles in both C and C++   */
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) abort();              /* allocation failed   */

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute all terms up to n (inclusive) and store them in the cache */
static void compute_until(idx_t n)
{
    if (n < 1) return;                      /* undefined for n<1 */
    size_t target = (size_t)n + 1;           /* next free index */
    if (target <= cache_len) return;         /* already computed */

    ensure_cache_capacity(n);

    size_t start = cache_len > 0 ? cache_len : 1;
    for (size_t i = start; i < target; ++i) {
        if (i <= 7) {
            cache[i] = init[i - 1];
        } else {
            /* recurrence formula */
            cache[i] = cache[i - 1]
                       + (seq_t)6 * cache[i - 2]
                       - (seq_t)6 * cache[i - 3]
                       - (seq_t)8 * cache[i - 4]
                       + (seq_t)8 * cache[i - 5]
                       + (seq_t)3 * cache[i - 6]
                       - (seq_t)3 * cache[i - 7];
        }
    }
    cache_len = target;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* out of bounds – safe fallback */
    compute_until(n);
    return cache[(size_t)n];
}
