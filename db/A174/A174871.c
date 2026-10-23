/*
 * A174871  Numbers k such that the k-th triangular number ends in 6 or 8.
 * https://oeis.org/A174871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174871 – numbers k such that the k‑th triangular number
   ends in 6 or 8.  The function returns the n-th such k (n >= 1). */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;    /* stored sequence values */
static size_t cache_size = 0;  /* number of values stored */
static size_t cache_cap = 0;   /* current allocation capacity */
static idx_t last_k = 0;       /* last integer examined */

/* dp for good modulo 20 pattern: true if T_k ends with 6 or 8 */
static const bool good_mod20[20] = {
    false, /* k mod 20 == 0  → T_k last digit 0  */
    false, /* 1               → 1  */
    false, /* 2               → 3  */
    true,  /* 3               → 6  */
    false, /* 4               → 0  */
    false, /* 5               → 5  */
    false, /* 6               → 1  */
    true,  /* 7               → 8  */
    true,  /* 8               → 6  */
    false, /* 9               → 5  */
    false, /* 10              → 5  */
    true,  /* 11              → 6  */
    true,  /* 12              → 8  */
    false, /* 13              → 1  */
    false, /* 14              → 5  */
    false, /* 15              → 0  */
    true,  /* 16              → 6  */
    false, /* 17              → 3  */
    false, /* 18              → 1  */
    false  /* 19              → 0  */
};

static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) exit(EXIT_FAILURE); /* allocation failure */
    cache = new_buf;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but return 0 for safety */

    /* Expand cache until we have at least n values */
    while (cache_size < (size_t)n) {
        ++last_k;
        if (good_mod20[(size_t)(last_k % 20)]) {
            /* Store the new qualifying index */
            if (cache_size == cache_cap) grow_cache();
            cache[cache_size++] = (seq_t)last_k;
        }
    }
    return cache[n - 1];
}
