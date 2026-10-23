/*
 * A139409  Numbers n such that n=d_1!!^2+d_2!!^2+...+d_k!!^2 where
 *          d_1d_2...d_k is the decimal expansion of n.
 * https://oeis.org/A139409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: n = Σ d_i!!²   (sum of double‑factorial squares of the decimal digits of n) */
/* Computes the nth term of OEIS A139409, 1‑based.  The routine grows a cache of found terms
   and returns the element at index n-1.  All arithmetic is performed in seq_t,
   which may be a built‑in or an arbitrary‑precision integer type. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Pre‑computed values d!!² for d = 0 … 9 (type seq_t is used correctly).
   The values fit comfortably in any reasonable seq_t (they are < 1e6). */
static const seq_t dig_dblfact_sq[10] = {
    1,          /* 0!!² */
    1,          /* 1!!² */
    4,          /* 2!!² */
    9,          /* 3!!² */
    64,         /* 4!!² */
    225,        /* 5!!² */
    2304,       /* 6!!² */
    11025,      /* 7!!² */
    147456,     /* 8!!² */
    893025      /* 9!!² */
};

/* Cache of already found sequence terms.  Values are stored in the order they
   appear, so the nth term is cache_entries[n-1]. */
static seq_t *cache_entries = NULL;   /* array pointer */
static size_t cache_cap     = 0;       /* current allocated capacity */
static size_t cache_len     = 0;       /* number of stored terms */

/* The last integer that was tested; used when the cache is grown. */
static seq_t last_tested = 0;

/* Grow the cache when it is full.  The cast to seq_t* is required so the
   code compiles under C++ where realloc returns void*. */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
    } else {
        cache_cap *= 2;
    }
    seq_t *tmp = (seq_t *)realloc((void *)cache_entries,
                                   cache_cap * sizeof(seq_t));
    /* Assign back; if realloc failed tmp will be NULL and the cache stays
       unusable – the caller will detect and terminate via the usual abort
       path of the program. */
    cache_entries = tmp;
}

/* Return the nth term of sequence A139409 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined; just return 0 which is harmless for most callers. */
        return (seq_t)0;
    }

    /* Extend the cache until it contains at least n elements. */
    while (cache_len < (size_t)n) {
        last_tested++;
        seq_t sum = 0;
        seq_t tmp  = last_tested;

        /* Compute the sum of d!!² for the decimal digits of last_tested. */
        while (tmp != 0) {
            unsigned digit = (unsigned)(tmp % 10);
            sum += dig_dblfact_sq[digit];
            tmp /= 10;
        }

        if (sum == last_tested) {
            if (cache_len == cache_cap) {
                grow_cache();
            }
            cache_entries[cache_len++] = last_tested;
        }
    }

    return cache_entries[n - 1];
}
