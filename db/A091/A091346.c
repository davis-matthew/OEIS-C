/*
 * A091346  Binomial convolution of A069321(n), where A069321(0)=0, with the
 *          sequence of all 1's alternating in sign.
 * https://oeis.org/A091346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (A000670(n+1) + (-1)^(n+1)) / 4 where A000670 is the
 *            Fubini (preferential arrangement) sequence. A000670(0) = 1, and
 *            A000670(n) = sum_{k=0}^{n-1} C(n, k) * A000670(k) (n >= 1) The
 *            function memoises A000670 values up to the requested index
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq_array.h"

static seq_t *fubini = NULL;   /* cached Fubini numbers */
static size_t  fubini_cap = 0; /* array capacity */
static size_t  fubini_len = 0; /* number of computed elements */

/* Compute and return the nth term of OEIS A091346. */
seq_t a(idx_t n)
{
    /* We need Fubini numbers up to index n+1. */
    size_t needed = (size_t)(n + 2);   /* indices 0 .. n+1 inclusive */

    /* Allocate or grow the cache if necessary. */
    if (fubini_cap < needed) {
        size_t old_cap = fubini_cap;
        grow_seq_array(&fubini, &fubini_cap, needed);
        /* initialise the new entries to zero */
        for (size_t i = old_cap; i < fubini_cap; ++i)
            fubini[i] = (seq_t)0;
    }

    /* Initialise the first Fubini number if this is the first call. */
    if (fubini_len == 0) {
        fubini[0] = (seq_t)1;
        fubini_len = 1;
    }

    /* Compute missing Fubini numbers up to index n+1. */
    for (idx_t idx = (idx_t)fubini_len; idx <= n + 1; ++idx) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;   /* C(idx,0) = 1 */

        /* sum_{k=0}^{idx-1} C(idx, k) * fubini[k] */
        for (idx_t k = 0; k < idx; ++k) {
            sum += binom * fubini[k];
            /* update binom: C(idx, k+1) = C(idx, k) * (idx - k) / (k+1) */
            binom = binom * (seq_t)(idx - k) / (seq_t)(k + 1);
        }
        fubini[idx] = sum;
        fubini_len = (size_t)(idx + 1);
    }

    /* a(n) = (Fubini(n+1) + (-1)^(n+1)) / 4 */
    seq_t val = fubini[n + 1];
    if ((n & 1) == 0)        /* n even -> (-1)^(n+1) = -1 */
        val -= (seq_t)1;
    else                     /* n odd  -> (-1)^(n+1) = +1 */
        val += (seq_t)1;
    val /= (seq_t)4;
    return val;
}
