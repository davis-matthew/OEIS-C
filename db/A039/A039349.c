/*
 * A039349  Numbers whose base-8 representation has the same nonzero number of
 *          3's and 6's.
 * https://oeis.org/A039349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039349: numbers whose base‑8 representation has the same
   nonzero number of 3's and 6's.  The function searches integers
   incrementally in base 8 and stores the found values in a static
   cache so that queries for any n≥1 are answered in O(1) after the
   needed terms have been generated. */

/*  Required utilities: usage of grow_seq_array to handle dynamic array
    allocation.  The function signature is:
        void grow_seq_array(seq_t **arr, size_t *size, size_t new_len);
*/
#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;   /* cache of found terms */
static size_t cache_size = 0; /* allocated size of cache */
static idx_t found_count = 0; /* number of terms in cache   */
static seq_t current = 1;     /* next integer to test       */

/* Ensure that at least 'n' terms are cached. */
static void ensure_cache(idx_t n)
{
    while (found_count < n) {
        seq_t x   = current;
        seq_t tmp = x;
        unsigned cnt3 = 0, cnt6 = 0;

        /* Count digits 3 and 6 in base 8 representation of 'x'. */
        while (tmp > 0) {
            unsigned digit = (unsigned)(tmp % (seq_t)8);
            if (digit == 3) ++cnt3;
            else if (digit == 6) ++cnt6;
            tmp /= (seq_t)8;
        }

        if (cnt3 > 0 && cnt3 == cnt6) {
            /* Resize cache if necessary. */
            grow_seq_array(&cache, &cache_size, found_count + 1);
            cache[found_count++] = x;
        }

        ++current;  /* test the next integer */
    }
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, but harmless */
    ensure_cache(n);
    return cache[n - 1];
}
