/*
 * A032970  Numbers with the property that all pairs of consecutive base-7
 *          digits differ by more than 1.
 * https://oeis.org/A032970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with the property that all pairs of consecutive
   base-7 digits differ by more than 1.
   Computation: generate the numbers in ascending order by increasing
   base‑7 length, using a depth‑first search that prunes pairs whose
   digits differ by ≤ 1.  The resulting list is stored in a static
   cache and filled lazily as larger n are requested.  This yields
   O(n) generation time for n up to about 10 000.  */

/* The following includes are required for the cache management
   and recursion helper functions.  Only stdlib.h and stdbool.h
   are used.  */
#include <stdlib.h>
#include <stdbool.h>

/* Static cache data */
static seq_t *cache = NULL;  /* array of generated values */
static size_t cap   = 0;     /* current allocated capacity */
static size_t len   = 0;     /* number of stored values */

/* Next base‑7 length that has not yet been generated.  */
static unsigned current_length = 1;

/* Ensure that the cache has space for at least 'need' elements. */
static void ensure_cap(size_t need)
{
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    }
    while (cap < need) {
        cap <<= 1; /* double the capacity */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
    }
}

/* Store a newly generated value in the cache. */
static void push_value(seq_t val)
{
    if (len >= cap) {
        ensure_cap(len + 1);
    }
    cache[len++] = val;
}

/* Recursive depth‑first search that builds all base‑7 numbers of a
   fixed length 'L'.  'pos'   : current position (0‑based),
   'val'    : value built so far in base‑10,
   'prev'   : digit placed at previous position (or -1 for none). */
static void gen_len_rec(unsigned pos, unsigned L, seq_t val, int prev)
{
    if (pos == L) {
        push_value(val);
        return;
    }

    for (int d = 0; d < 7; ++d) {
        if (prev == -1) {              /* leading digit, cannot be 0 */
            if (d == 0) continue;
        } else {                       /* subsequent digits must differ by > 1 */
            if (abs(d - prev) <= 1) continue;
        }
        gen_len_rec(pos + 1, L, val * 7 + d, d);
    }
}

/* Generate all numbers of a given base‑7 length 'L'. */
static void generate_length(unsigned L)
{
    gen_len_rec(0, L, 0, -1);
}

/* Main sequence function.  The cache is grown lazily until the
   requested index 'n' is available.  Array indices are zero‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; value for n==0 is arbitrary */

    size_t target = (size_t)n;

    /* Grow the cache until we have the desired element. */
    while (len < target) {
        generate_length(current_length);
        ++current_length;
    }

    return cache[n - 1];
}
