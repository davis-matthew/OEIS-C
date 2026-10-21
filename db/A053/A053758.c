/*
 * A053758  a(n) = a(a(n-5))+a(n-a(n-5)) if n>2 otherwise a(n)=1.
 * https://oeis.org/A053758
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(a(n-5)) + a(n - a(n-5)) for n > 2, * a(n) = 1 otherwise
 *            * Offset: -2 (a(-2) is the first term)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;     /* memoisation table for n >= -2   */
static size_t  cap   = 0;       /* current capacity of the array   */
static const   idx_t OFFSET = -2; /* n == -2 corresponds to index 0  */

/* Make sure the cache can hold the value for index 'n'.   */
static void ensure_cache(idx_t n)
{
    idx_t   pos = n - OFFSET;                     /* position in array */
    if ((size_t)pos >= cap) {                     /* need more space? */
        size_t new_cap = cap ? cap * 2 : 16;       /* growth policy     */
        while ((size_t)pos >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                        /* allocation failed */
        /* Zero‑initialize newly added slots.  Works for integer
         * types and for user‑defined arbitrary‑precision classes
         * that support assignment of zero.                */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }
}

/* Public sequence function.                                  */
seq_t a(idx_t n)
{
    /* Base values: a(n) = 1 for n <= 2.  The offset guarantees
     * that positions for n <= 2 are not stored in the cache yet. */
    if (n <= 2) return (seq_t)1;

    /* Ensure the memoisation array can contain this index.   */
    ensure_cache(n);

    size_t pos = (size_t)(n - OFFSET);
    if (cache[pos] != 0)            /* already computed?      */
        return cache[pos];

    /* Recursively compute the two subterms required by the definition. */
    seq_t sub = a(n - (idx_t)5);                /* a(n-5)             */
    seq_t left = a((idx_t)sub);                /* a(a(n-5))          */
    idx_t idx2 = n - (idx_t)sub;               /* n - a(n-5)         */
    seq_t right = a(idx2);                     /* a(n - a(n-5))      */

    seq_t val = left + right;                  /* recurrence value   */
    cache[pos] = val;                          /* store for reuse    */
    return val;
}
