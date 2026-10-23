/*
 * A186468  Number of (n+1)X6 0..2 arrays with every 2X2 subblock commuting
 *          with each of its horizontal and vertical 2X2 subblock neighbors.
 * https://oeis.org/A186468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 26, u64 n <= 63, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A186468: Number of (n+1)×6 0..2 arrays with every 2×2 subblock commuting
   with each of its horizontal and vertical 2×2 neighbors.
   Recurrence (for n>14):
       a(n) = 2*a(n‑1) + a(n‑2) – 4*a(n‑3) + 4*a(n‑4) – 2*a(n‑5)
   Initial values:
       a(1)=966, a(2)=903, a(3)=3190, a(4)=4875, a(5)=13278,
       a(6)=16083, a(7)=40626, a(8)=52755, a(9)=129102,
       a(10)=182841, a(11)=418022, a(12)=632281, a(13)=1362100,
       a(14)=2157562.
   The implementation uses a lazy memoised array and computes terms
   on demand, so that any call order is supported and each term is
   produced exactly once. */

#include <stdlib.h>

static const seq_t a_initial[15] = {                           /* 1‑based */
    0,      /* unused index 0 */
    966,    /* a(1) */
    903,    /* a(2) */
    3190,   /* a(3) */
    4875,   /* a(4) */
    13278,  /* a(5) */
    16083,  /* a(6) */
    40626,  /* a(7) */
    52755,  /* a(8) */
    129102, /* a(9) */
    182841, /* a(10) */
    418022, /* a(11) */
    632281, /* a(12) */
    1362100,/* a(13) */
    2157562 /* a(14) */
};

static seq_t *a_cache = NULL;   /* 1‑based cache, NULL = uninitialised */
static idx_t  a_cache_max = 0;  /* highest index that currently holds a valid value */

/* Ensure the cache is allocated and contains the known initial values
   up to index 14.  After this call, a_cache_max is at least 14. */
static void a_init_cache(void)
{
    if (a_cache != NULL) return;                 /* already initialised */

    const size_t cap = 15;                       /* indices 0..14 */
    a_cache = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!a_cache) exit(1);
    for (size_t i = 0; i < cap; ++i) a_cache[i] = 0;

    for (size_t i = 1; i <= 14; ++i) a_cache[i] = a_initial[i];
    a_cache_max = 14;
}

/* Grow the cache array to at least `new_cap` (1‑based). */
static void a_grow_cache(idx_t new_cap)
{
    if (a_cache_max >= new_cap) return;          /* already sufficient */

    size_t cap = (size_t)new_cap + 1;            /* include index 0 */
    seq_t *tmp = (seq_t*)realloc(a_cache, cap * sizeof(seq_t));
    if (!tmp) exit(1);
    a_cache = tmp;
    /* initialise newly allocated slots to 0 */
    for (size_t i = a_cache_max + 1; i < cap; ++i) a_cache[i] = 0;

    a_cache_max = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n=0, safe default */

    a_init_cache();                   /* make sure cache exists */

    if (n <= a_cache_max)             /* value already computed */
        return a_cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = a_cache_max + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 14) {                /* safety: initial values already set */
            val = a_initial[i];
        } else {
            val = 2 * a_cache[i - 1]
                + a_cache[i - 2]
                - 4 * a_cache[i - 3]
                + 4 * a_cache[i - 4]
                - 2 * a_cache[i - 5];
        }
        a_grow_cache(i);              /* ensure space for i */
        a_cache[i] = val;
    }

    a_cache_max = n;                   /* all terms up to n are ready */
    return a_cache[n];
}
