/*
 * A170526  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170526 – Reduced words in a Coxeter group of rank 37
 *
 *  For n≥0 the numbers satisfy the linear recurrence obtained from the
 *  generating function
 *
 *          (t^46 + 2 t^45 + … + 2 t + 1)
 *  A(t) = -------------------------------------------------
 *          (630 t^46 – 35 t^45 – … – 35 t + 1)
 *
 *  Denominator: 1 – 35 t – … – 35 t^45 + 630 t^46
 *
 *  Recurrence:
 *
 *        a(n) – 35(a(n‑1)+…+a(n‑45)) + 630 a(n‑46) = p(n)
 *
 *  where the numerator coefficients are
 *        p(0)=1,
 *        p(46)=1,
 *        p(n)=2 for 1≤n≤45,
 *        p(n)=0 for n>46.
 *
 *  Consequently
 *
 *        a(n) = 35 Σ_{i=1}^{45} a(n-i) – 630 a(n-46) + p(n)      (1)
 *
 *  The first value is a(0)=1, and the recurrence yields all further
 *  terms.  The implementation below caches previously computed terms
 *  so that any number of calls with arbitrary orders of indices is
 *  efficient (O(n) time in total for all calls up to the largest n
 *  requested).
 */

#include <stddef.h>
#include <stdlib.h>

/* We rely on the typedefs seq_t and idx_t being provided by the build. */

static seq_t *cache = NULL;      /* cached values a(0), a(1), … */
static size_t cache_len  = 0;    /* number of values currently stored   */
static size_t cache_cap  = 0;    /* allocated capacity                   */

/* enlarge the cache so that indices 0 … needed-1 are available   */
static void
ensure_cache(size_t needed)
{
    if (needed <= cache_len)
        return;

    /* simple exponential growth */
    size_t new_cap = (cache_cap ? cache_cap : 1);
    while (new_cap < needed) {
        new_cap = new_cap * 2;
    }
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – abort */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
    /* new elements are uninitialized; they will be written before use */
}

/*  Main function – public interface required by the framework   */
seq_t
a(idx_t n)
{
    /* Initialise cache storage on first call */
    if (cache_len == 0) {
        ensure_cache(1);         /* start with capacity for a(0) */
    }

    /* Extend cache if necessary */
    if ((size_t)n + 1 > cache_len) {
        ensure_cache((size_t)n + 1);
    }

    /* Compute any terms that haven't been cached yet */
    for (idx_t k = (idx_t)cache_len; k <= n; ++k) {
        /* sum of the 45 previous values (zero for negative indices) */
        seq_t sum = 0;
        for (idx_t i = 1; i <= 45; ++i) {
            if (k >= i) {
                sum += cache[k - i];
            }
        }

        /* base recurrence part */
        seq_t term = sum * 35;

        /* subtract the a(n‑46) contribution for k >= 46 */
        if (k >= 46) {
            term -= cache[k - 46] * 630;
        }

        /* add numerator coefficient p(k) */
        if (k == 0) {
            term += 1;              /* p(0) = 1 */
        } else if (k == 46) {
            term += 1;              /* p(46) = 1 */
        } else if (k <= 45) {
            term += 2;              /* p(1)…p(45) = 2 */
        } /* for k>46, p(k)=0 – nothing to add */

        cache[k] = term;
    }

    return cache[(size_t)n];
}
