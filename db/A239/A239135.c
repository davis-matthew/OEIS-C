/*
 * A239135  Numbers k such that (k-1)*k^2 + 1 and k^2 + (k-1) are both prime.
 * https://oeis.org/A239135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 641, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A239135: integers k such that
   (k-1)*k^2 + 1  and  k^2 + (k-1) are both prime.
   The function searches incrementally and stores found terms
   in a cache so that any index may be requested in any order. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq.h"

/* Static cache for previously found terms */
static seq_t *cache   = NULL;  /* array of cached values */
static size_t sz      = 0;     /* number of terms stored */
static size_t cap     = 0;     /* allocated capacity */

/* Append a new term, growing the cache if necessary. */
static void append_term(seq_t val)
{
    if (sz == cap)
        grow_seq(&cache, &cap, (cap ? cap * 2 : 16));
    cache[sz++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        abort();                 /* n must be at least 1 */

    /* Return cached value if already available. */
    if ((size_t)n <= sz)
        return cache[n - 1];

    /* Start searching from the first integer after the last cached term. */
    idx_t start = sz ? (idx_t)cache[sz - 1] + 1 : 2;

    /* Search until we have found the nth term. */
    for (idx_t k = start; sz < (size_t)n; ++k) {
        seq_t k_seq = (seq_t)k;               /* cast to seq_t for arithmetic */
        seq_t k2    = k_seq * k_seq;          /* k^2 */
        seq_t num2  = k2 + k_seq - 1;         /* k^2 + (k-1) */
        seq_t num1  = (k_seq - 1) * k2 + 1;    /* (k-1)*k^2 + 1 */

        if (isPrimeGeneric(num1) && isPrimeGeneric(num2))
            append_term(k_seq);
    }

    return cache[n - 1];
}
