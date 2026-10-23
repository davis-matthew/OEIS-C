/*
 * A039026  Numbers whose base-6 representation has the same number of 2's and
 *          5's.
 * https://oeis.org/A039026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-6 representation has the same number of 2's and 5's.
   The function memoises the sequence in a dynamically resizable array.
   It works for uint64_t, int64_t and for the arbitrary‑precision class
   used in the big build. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of sequence terms */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* number of terms stored */

/* Ensure that at least `need` terms are in the cache.  `need` is 1‑based
   – that is, after the call we have cache_len ≥ need. */
static void
ensure_cache(size_t need)
{
    if (cache_len >= need)
        return;                 /* already enough */

    /* enlarge allocation if required */
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate further terms up to `need` */
    seq_t candidate = cache_len ? cache[cache_len - 1] + 1 : 0;
    while (cache_len < need) {
        seq_t v = candidate;
        unsigned c2 = 0, c5 = 0;

        if (v != 0) {
            while (v != 0) {
                unsigned digit = (unsigned)(v % 6);
                if (digit == 2) ++c2;
                else if (digit == 5) ++c5;
                v /= 6;
            }
        }
        /* 0 has 0 twos and 0 fives – counts match */
        if (c2 == c5)
            cache[cache_len++] = candidate;
        ++candidate;
    }
}

/* Sequence function: a(n) – first term is a(1).  The implementation
   does not assume monotone calls of `n`. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; a(0) is not defined */
        return 0;
    ensure_cache((size_t)n);
    return cache[n - 1];
}
