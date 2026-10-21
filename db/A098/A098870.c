/*
 * A098870  Sum of the cubes of the digits of the previous term, starting with
 *          2.
 * https://oeis.org/A098870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the cubes of the decimal digits of a(n-1); a(1) =
 *            2
 * Range:     u8 n <= 2, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes past values in a static array so that
   successive calls with arbitrary n remain O(n) overall.  The
   calculation itself is trivial: for each term we loop through its
   decimal digits and accumulate digit³.  All intermediate values
   use the seq_t type, which is guaranteed to support the required
   operations. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* 1‑based cache for sequence values */
static idx_t  cache_size = 0;        /* current allocated size (elements) */
static idx_t  computed_upto = 0;     /* highest index computed so far */

static inline seq_t next_term(seq_t prev)
{
    seq_t sum = 0;
    seq_t v = prev;
    while (v > 0) {
        unsigned digit = (unsigned)(v % 10);
        seq_t c = (seq_t)digit * digit * digit;
        sum += c;
        v /= 10;
    }
    /* if prev == 0, the loop is skipped and sum stays 0, as desired */
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but return 0 for safety */

    /* Allocate or enlarge cache if necessary */
    if (cache_size == 0) {
        cache_size = n + 10; /* initial size with headroom */
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
    } else if (n + 5 > cache_size) {
        size_t new_size = cache_size * 2;
        if (new_size < (size_t)(n + 5))
            new_size = n + 5;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL) abort(); /* allocation failure */
        cache_size = new_size;
    }

    /* Initialise first value if not already */
    if (computed_upto == 0) {
        cache[1] = (seq_t)2;
        computed_upto = 1;
    }

    /* Compute forward until we reach n */
    while (computed_upto < n) {
        cache[computed_upto + 1] = next_term(cache[computed_upto]);
        ++computed_upto;
    }

    return cache[n];
}
