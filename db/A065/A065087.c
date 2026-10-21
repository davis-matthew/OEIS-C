/*
 * A065087  a(n) = A000166(n)*binomial(n+1,2).
 * https://oeis.org/A065087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000166(n) * binomial(n+1,2) (derangement * n*(n+1)/2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Static cache for derangement values !k */
static seq_t *der_cache = NULL;   /* array of derangement numbers */
static size_t der_len   = 0;       /* number of entries computed so far */
static size_t der_cap   = 0;       /* capacity of der_cache */

/* Make sure the der_cache can hold at least `need + 1` entries (0 … need) */
static void ensure_der_capacity(size_t need)
{
    if (need < der_cap) return;
    size_t new_cap = der_cap ? der_cap * 2 : 1;
    while (new_cap <= need) new_cap <<= 1;

    if (!der_cache) {
        der_cache = seq_alloc(new_cap);             /* allocate new array */
    } else {
        der_cache = seq_realloc(der_cache, der_cap, new_cap); /* resize */
    }
    der_cap = new_cap;
}

/* a(n) = !n * n*(n+1)/2   for n >= 0 */
seq_t a(idx_t n)
{
    /* For n = 0 or 1 the value is zero */
    if (n < 2) return 0;

    /* Initialise cache on first call */
    if (!der_cache) {
        ensure_der_capacity(1);          /* need entries 0 and 1 */
        der_cache[0] = (seq_t)1;         /* !0 = 1 */
        der_cache[1] = (seq_t)0;         /* !1 = 0 */
        der_len = 2;
    }

    /* Compute derangements up to n if not yet done */
    if (der_len <= n) {
        ensure_der_capacity((size_t)n);
        for (size_t i = der_len; i <= (size_t)n; ++i) {
            seq_t sum = der_cache[i-1] + der_cache[i-2];
            der_cache[i] = ((seq_t)(i-1)) * sum;
        }
        der_len = (size_t)n + 1;
    }

    /* binomial(n+1,2) = n*(n+1)/2  (compute using seq_t) */
    seq_t bin = ((seq_t)n * (seq_t)(n + 1)) / 2;

    return der_cache[(size_t)n] * bin;
}
