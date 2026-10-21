/*
 * A209367  Number of n X 1 1..3 arrays with no element with value z exactly a
 *          city block distance of z from another element with value z.
 * https://oeis.org/A209367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 36, u64 n <= 75, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A209367: Number of n×1 arrays with values 1..3 and no two equal
   values z exactly at Manhattan distance z.
   Empirical recurrence (valid for all n>14):
   a(n) = a(n‑2) + 4·a(n‑4) + 4·a(n‑5) + 5·a(n‑6)
          – a(n‑7) – 5·a(n‑8) – 5·a(n‑9) – 5·a(n‑10)
          + a(n‑11) + 2·a(n‑12)
   Initial values:
   a(1)=3,a(2)=8,a(3)=19,a(4)=35,a(5)=64,a(6)=110,
   a(7)=185,a(8)=322,a(9)=599,a(10)=1084,a(11)=1902,
   a(12)=3280,a(13)=5816,a(14)=10425   */

#include <stdlib.h>

/* cached computed terms; indices 0..n‑1 contain a(1)..a(n) */
static seq_t *cache = NULL;
/* number of allocated slots in cache */
static size_t cache_capacity = 0;
/* highest n that has been computed so far (1‑based) */
static size_t computed_up_to = 0;

/* initial values a(1)..a(14) in 0‑based array order */
static const seq_t init_vals[14] = {
    3, 8, 19, 35, 64, 110,
    185, 322, 599, 1084, 1902, 3280,
    5816, 10425
};

/* ensure that cache has at least n slots and that the first 14
   indices are initialized with the known values */
static void ensure_cache(idx_t n)
{
    if (n == 0)
        return;

    if (cache != NULL && n <= (idx_t)cache_capacity)
        return;

    size_t new_cap = (cache_capacity == 0) ? 16 : cache_capacity;
    while ((idx_t)new_cap < n)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* allocation failure */

    cache = tmp;
    cache_capacity = new_cap;

    /* store the 14 initial terms if not already present */
    if (computed_up_to < 14) {
        size_t start = computed_up_to < 14 ? computed_up_to : 14;
        for (size_t i = start; i < 14; ++i)
            cache[i] = init_vals[i];
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* offset 1; not used in tests */

    ensure_cache(n);

    /* compute all terms up to n that are not yet present */
    while (computed_up_to < (size_t)n) {
        ++computed_up_to;
        size_t i = computed_up_to;      /* 1‑based index */

        if (i <= 14) {
            /* first 14 values already stored by ensure_cache */
            continue;
        }

        /* recurrence computation using 0‑based cache indices */
        seq_t pos =
            cache[i - 3]                 /* a(n-2)  */
          + (seq_t)4 * cache[i - 5]      /* 4·a(n-4) */
          + (seq_t)4 * cache[i - 6]      /* 4·a(n-5) */
          + (seq_t)5 * cache[i - 7]      /* 5·a(n-6) */
          + cache[i - 12]               /* a(n-11) */
          + (seq_t)2 * cache[i - 13];    /* 2·a(n-12) */

        seq_t neg =
            cache[i - 8]                 /* a(n-7)  */
          + (seq_t)5 * cache[i - 9]      /* 5·a(n-8) */
          + (seq_t)5 * cache[i - 10]     /* 5·a(n-9) */
          + (seq_t)5 * cache[i - 11];    /* 5·a(n-10) */

        cache[i - 1] = pos - neg;       /* store a(n) */
    }

    return cache[(size_t)n - 1];
}
