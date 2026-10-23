/*
 * A125749  a(n) is the smallest positive integer such that (Sum_{1<=k<=a(n),
 *          gcd(t(k),n)=1} t(k)) is >= n, where t(k) is the k-th positive
 *          integer which is coprime to n.
 * https://oeis.org/A125749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer k such that the sum of the first k
 *            integers coprime to n is at least n
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *a_cache = NULL;   /* cache for precomputed values */
static idx_t  a_cache_cap = 0;  /* current cache capacity */

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but avoid crash */

    /* Grow cache if necessary */
    if (n > a_cache_cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        /* initialize newly added cache entries to 0 (meaning "not yet computed") */
        for (idx_t i = a_cache_cap; i < new_cap; ++i) tmp[i] = 0;
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (a_cache[n - 1] != 0) return a_cache[n - 1];

    seq_t sum   = 0;                 /* running sum of coprime integers */
    seq_t count = 0;                 /* number of coprime integers seen so far */
    seq_t n_seq = (seq_t)n;          /* n promoted to seq_t for arithmetic */

    for (idx_t k = 1; ; ++k) {
        if (gcd((seq_t)k, n_seq) == 1) {
            sum += (seq_t)k;
            ++count;
            if (sum >= n_seq) break;
        }
    }

    a_cache[n - 1] = count;
    return count;
}
