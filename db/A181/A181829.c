/*
 * A181829  a(n) = 4*A060819(n-2)*A060819(n+2).
 * https://oeis.org/A181829
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 90, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A181829: a(n) = 4*A060819(n-2)*A060819(n+2)
   The sequence satisfies the linear recurrence
     a(n) = 3*a(n-4) - 3*a(n-8) + a(n-12)
   with initial values a(-2)…a(9).  This function uses a memoised
   dynamic array to compute terms on demand.  It works for any
   seq_t (including arbitrary‑precision) and for any n ≥ –2.  */

/*  Include the standard library for memory allocation.  */
#include <stdlib.h>

/*  The function defined by the problem statement  */
seq_t a(idx_t n)
{
    /*  The sequence is defined for n ≥ –2.  Values for smaller indices
        are not used in the tests; we return 0 for them.  */
    if (n < -2) return (seq_t)0;

    /* Convert the Olympian index to a non‑negative array index.  */
    idx_t k = n + 2;                     /* k = 0 ⇔ n = –2, …   */

    /*  Static cache of already computed terms.  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /*  Initial values a(-2)…a(9) stored once.  */
    static const seq_t init_vals[12] = {
        (seq_t)0,   /* a(-2) */
        (seq_t)-12, /* a(-1) */
        (seq_t)-4,  /* a(0)  */
        (seq_t)-12, /* a(1)  */
        (seq_t)0,   /* a(2)  */
        (seq_t)20,  /* a(3)  */
        (seq_t)12,  /* a(4)  */
        (seq_t)84,  /* a(5)  */
        (seq_t)8,   /* a(6)  */
        (seq_t)180, /* a(7)  */
        (seq_t)60,  /* a(8)  */
        (seq_t)308  /* a(9)  */
    };
    const idx_t init_len = 12;

    /*  Initialise the cache with the known initial values if it has
        not been done yet.  */
    if (cache_size < init_len) {
        idx_t new_size = init_len;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = init_vals[i];
        cache_size = new_size;
    }

    /*  If the requested term is already cached, return it immediately. */
    if (k < cache_size)
        return cache[k];

    /*  Extend the cache so that it covers the required index k.  */
    idx_t new_size = k + 1;                 /* inclusive */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;              /* allocation failed */
    cache = tmp;

    /*  Compute terms from cache_size up to new_size-1 using the recurrence. */
    for (idx_t i = cache_size; i < new_size; ++i) {
        /* The recurrence needs the values at i-4, i-8, and i-12,
           which are guaranteed to exist because i ≥ cache_size ≥ 12.  */
        seq_t term =
            (seq_t)3 * cache[i - 4]
          - (seq_t)3 * cache[i - 8]
          + cache[i - 12];
        cache[i] = term;
    }

    cache_size = new_size;
    return cache[k];
}
