/*
 * A043309  a(n)=A033003(n)/6.
 * https://oeis.org/A043309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th integer whose base‑25 representation uses only
   digits 0‑4 and has no two adjacent equal digits. */

#include <stdint.h>
#include <stdlib.h>

/* Static cache of already generated terms.  The array is
   resized with malloc/realloc, and elements are stored in the order
   required by OEIS. */
static seq_t    *cache     = NULL;   /* array of values          */
static idx_t    cache_size = 0;       /* number of values stored  */
static idx_t    cache_cap  = 0;       /* allocated capacity       */
static int      last_len   = 0;       /* length of longest numbers
                                         generated so far        */

/* Append a new value to the cache, expanding the allocation if
   necessary.  The cast on realloc is required for C++ compatibility. */
static void append_cache(seq_t v)
{
    if (cache_size == cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = v;
}

/* Recursive helper that builds the remaining digits of a number with
   a fixed total length.  'remaining' is how many digits still need to
   be chosen, 'prev' is the last chosen digit (ensuring no duplicate
   adjacency), and 'val' is the value constructed so far. */
static void generate_rest(int remaining, int prev, seq_t val)
{
    if (remaining == 0) {
        append_cache(val);
        return;
    }
    for (int d = 0; d <= 4; ++d) {
        if (d == prev) continue;
        generate_rest(remaining - 1, d, val * (seq_t)25 + d);
    }
}

/* Generate all numbers with exactly 'len' base‑25 digits (len ≥ 1)
   and append them to the cache.  The first digit cannot be zero. */
static void generate_len(int len)
{
    for (int d = 1; d <= 4; ++d) {
        generate_rest(len - 1, d, d);
    }
}

/* Ensure that at least 'n' terms are present in the cache. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n) return;

    for (int len = last_len + 1; ; ++len) {
        generate_len(len);
        last_len = len;
        if (cache_size >= n) break;
    }
}

/* Public function as required by the framework.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, behaviour for n=0 is unused */
    ensure_cache(n);
    return cache[n - 1];
}
