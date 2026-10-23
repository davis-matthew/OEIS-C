/*
 * A115605  Expansion of -x^2*(2 + x - 2*x^2 - x^3 + 2*x^4) / ( (x-1)*(1+x)*(1
 *          + x + x^2)*(x^2 - x + 1)*(x^2 + 4*x - 1)*(x^2 - x - 1) ).
 * https://oeis.org/A115605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A115605:  Expansion of -x^2*(2 + x - 2*x^2 - x^3 + 2*x^4)
    /((x-1)*(1+x)*(1+x+x^2)*(x^2-x+1)*(x^2+4*x-1)*(x^2-x-1))

    The generating function is rational, hence the coefficients satisfy a
    linear recurrence of order 10.  With Q(x) = \sum_{k=0}^{10} q_k x^k
    and P(x) = \sum_{n=0}^{6} p_n x^n we have
            a(n) = \sum_{k=1}^{10} q_k a(n-k) – p(n),
    where q = [-1, 3, 6, -3, -1, 0, 1, -3, -6, 3, 1]  (q[0] is unused)
    and p = [0, 0, -2, -1, 2, 1, -2] (zero for n>6).
    Initially a(0)=0, a(1)=0, all other a(n) are produced by this recurrence.
    The implementation memoises all terms up to the largest n requested
    so that any order of calls is efficient.

    The code uses only integer arithmetic; multiplications by small
    (negative or positive) constants are performed with explicit sign
    handling to avoid overflow on unsigned seq_t types.  The array of
    previous terms is grown as needed and all indices are safely cast
    to size_t.  The function is thread‑safe only for single‑threaded
    usage; concurrent calls would need external synchronisation.  */

#include <stdlib.h>

static const int q[11] = {0, 3, 6, -3, -1, 0, 1, -3, -6, 3, 1};
static const int p[7]  = {0, 0, -2, -1, 2, 1, -2};

static seq_t *cache      = NULL;   /* memoised a(n) values   */
static size_t cache_size = 0;      /* number of stored terms */
static size_t cache_cap  = 0;      /* current allocation size */

/* Ensure capacity for at least new_cap elements in cache. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_cap2 = cache_cap > 0 ? cache_cap * 2 : 16;
    if (new_cap2 < new_cap)
        new_cap2 = new_cap;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap2 * sizeof(seq_t));
    if (!new_ptr)
        exit(EXIT_FAILURE);   /* out of memory */
    cache = new_ptr;
    cache_cap = new_cap2;
}

/* Main function requested by the build system */
seq_t a(idx_t n)
{
    /* Convert n to a native size_t (safe for n <= 10000). */
    size_t idx = (size_t)n;

    /* Grow cache up to idx. */
    if (idx >= cache_size) {
        ensure_capacity(idx + 1);

        /* Fill missing entries using the recurrence. */
        for (size_t i = cache_size; i <= idx; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;

            for (int k = 1; k <= 10 && i >= (size_t)k; ++k) {
                int qk = q[k];
                seq_t val = cache[i - k];
                if (qk > 0)
                    pos += (seq_t)qk * val;
                else
                    neg += (seq_t)(-qk) * val;
            }

            seq_t val = pos;
            if (neg != 0)
                val -= neg;

            /* subtract p(i) if present */
            if (i <= 6) {
                int pk = p[i];
                if (pk > 0)
                    val -= (seq_t)pk;
                else
                    val += (seq_t)(-pk);
            }

            cache[i] = val;
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
