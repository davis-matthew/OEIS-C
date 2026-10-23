/*
 * A044445  Numbers n such that string 0,3 occurs in the base 4 representation
 *          of n but not of n+1.
 * https://oeis.org/A044445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series: numbers n such that the base‑4 representation of n contains the
   consecutive digits 0,3 but the base‑4 representation of n+1 does not.
   Computed by brute‑force search. */
#include <stdlib.h>
#include <stdbool.h>

/* Cache for memoised terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of cached terms */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Return true if the base‑4 representation of x contains the substring
   "03" (i.e. a 0 immediately followed by a 3 from most significant to
   least). */
static bool has_03(seq_t x)
{
    /* Store least‑significant digits first; a 0 followed by a 3 in the
       usual order corresponds to digits[i+1] == 0 && digits[i] == 3. */
    unsigned digits[256];
    size_t dlen = 0;

    if (x == 0) {
        digits[0] = 0;
        dlen = 1;
    } else {
        while (x != 0) {
            digits[dlen++] = (unsigned)(x % 4);
            x = x / 4;
        }
    }

    for (size_t i = 0; i + 1 < dlen; ++i) {
        if (digits[i + 1] == 0 && digits[i] == 3)
            return true;
    }
    return false;
}

/* Return the nth term of A044445 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n = 0, but avoid UB */
        return 0;

    /* Generate terms until we have at least n terms cached. */
    while (cache_size < n) {
        seq_t candidate;
        if (cache_size == 0)
            candidate = 19;           /* first term is known */
        else
            candidate = cache[cache_size - 1] + 1;

        /* Search for the next qualifying number. */
        for (;;) {
            if (has_03(candidate) && !has_03(candidate + 1)) {
                /* Grow cache if necessary. */
                if (cache_size == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 8;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_size++] = candidate;
                break;
            }
            candidate = candidate + 1;
        }
    }

    return cache[n - 1];
}
