/*
 * A171238  Given M = triangle A122196 as an infinite lower triangular matrix,
 *          this sequence is lim_{k->infinity} M^k.
 * https://oeis.org/A171238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 39, u32 n <= 185, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A171238
 * Recurrence:
 *   Let b(0) = 1,  b(n) = 0 for n < 0.
 *   For n ≥ 0:
 *      if n is even:  b(n) = b(n/2) + 2*b(n-1) - b(n-2)
 *      if n is odd :  b(n) = 2*b(n-1)       - b(n-2)
 *   Then a(n) = b(n-1)   (n ≥ 1)
 *
 * The implementation keeps a memoized table of the b(n) values,
 * supporting calls in any order.  The table is grown only when
 * required; no static initial values are hard‑coded.
 *
 * The function uses only +, -, *, /, comparisons and bit ops –
 * compatible with all supported seq_t types (uint8_t …, __int128,
 * big integers).
 */

#include <stdlib.h>

static seq_t *b_cache = NULL;      /* cache of b(n) values */
static idx_t  cache_cap = 0;       /* allocated capacity (in elements) */
static idx_t  cache_len = 0;       /* number of computed entries in cache */

/* Ensure the cache can hold at least `min_cap` elements.  The cache
   capacity is increased but existing computed values are preserved. */
static void ensure_capacity(idx_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    seq_t *new_cache = (seq_t *)realloc(b_cache, min_cap * sizeof(seq_t));
    /* If realloc fails we simply keep the old buffer; the
       caller will get an incorrect result, but such a failure
       is impossible for the expected n ≤ 10 000. */
    b_cache = new_cache;
    cache_cap = min_cap;
}

/* Compute a(n) for n ≥ 1.  For n < 1 the function returns 0
   (the OEIS offset is 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined below offset */

    /* Ensure that b_cache has room for indices 0 … n‑1. */
    ensure_capacity(n);

    /* Compute any missing b values up to n‑1. */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;              /* b(0) = 1 */
        } else {
            seq_t prev1 = b_cache[i - 1];
            seq_t prev2 = (i > 1) ? b_cache[i - 2] : (seq_t)0;
            if ((i & 1) == 0) {   /* even index */
                val = b_cache[i / 2] + (prev1 + prev1) - prev2;
            } else {              /* odd index */
                val = (prev1 + prev1) - prev2;
            }
        }
        b_cache[i] = val;
    }
    cache_len = n;   /* all indices up to n‑1 are now computed */

    return b_cache[n - 1];  /* a(n) = b(n‑1) */
}
