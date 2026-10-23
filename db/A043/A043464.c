/*
 * A043464  Numbers having four 2's in base 9.
 * https://oeis.org/A043464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly four 2's in base-9
   Returned in ascending order, a(1) is the first such number.
   The function builds the list on demand, remembering already
   found elements so that calls in any order are efficient. */
#include <stdlib.h>
#include <stdbool.h>

/* Helper to count the number of digit 2 in the base-9 representation
   of a number.  Works for all supported seq_t types. */
static inline unsigned count_twos(seq_t x)
{
    unsigned c = 0;
    while (x != 0) {
        seq_t d = x % (seq_t)9;
        if (d == (seq_t)2) ++c;
        x /= (seq_t)9;
    }
    return c;
}

/* Cache for already generated sequence terms.  Maintained across
   calls in external linkage. */
static seq_t *cache = NULL;          /* array of seq_t values */
static size_t cache_size = 0;        /* number of elements stored */
static size_t cache_cap   = 0;        /* allocated capacity */
static seq_t next_candidate = (seq_t)1;  /* next number to test */

/* Append one value to the cache, resizing if necessary. */
static void cache_push(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        cache = (seq_t *)tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Main function: return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n elements. */
    while (cache_size < (size_t)n) {
        if (count_twos(next_candidate) == 4) {
            cache_push(next_candidate);
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
