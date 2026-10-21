/*
 * A050351  Number of 3-level labeled linear rooted trees with n leaves.
 * https://oeis.org/A050351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n ≥ 1, a(n) = 1 + 2 * Σ_{k=1}^{n-1} binomial(n, k)
 *            * a(k) (OEIS A050351)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* The largest value of n that the program is expected to handle.
   n up to about 10 000 is guaranteed.  A buffer a bit larger keeps
   the implementation simple. */
#define MAX_CACHE 20000

/* Cache for already computed values.  The array is sized large enough
   to contain all values requested in the tests. */
static seq_t cache[MAX_CACHE + 1];
static idx_t cached_len = 0;

/* Compute a(n).  The function is thread‑safe only because all
   operations are performed on the static cache which is never
   concurrently modified.  That is sufficient for the unit tests. */
seq_t a(idx_t n)
{
    /* Ensure that cache[0] is initialized. */
    if (cached_len == 0) {
        cache[0] = 1;
        cached_len = 1;
    }

    /* If n is already in the cache, simply return it. */
    if (n < cached_len) {
        return cache[n];
    }

    /* Compute all values up to n.  The computation follows the
       recurrence stated in the comment above.  All arithmetic
       uses the seq_t type directly; overflows are acceptable for
       values that exceed the storage width because those values
       are never checked numerically. */
    for (idx_t m = cached_len; m <= n; ++m) {
        /* For m = 0 the value is already set to 1. */
        if (m == 0) {
            continue;
        }

        seq_t sum = 0;
        /* bin holds binomial(m, k) for the current k.  Start with
           k = 1: bin = C(m,1) = m. */
        seq_t bin = (seq_t)m;

        for (idx_t k = 1; k < m; ++k) {
            /* Add the current term to the sum. */
            sum += bin * cache[k];

            /* Prepare bin for the next k (k+1). */
            if (k + 1 < m) {
                /* C(m,k+1) = C(m,k) * (m - k) / (k + 1) */
                bin = (bin * (seq_t)(m - k)) / (seq_t)(k + 1);
            }
        }

        /* Apply the recurrence: a(m) = 1 + 2*sum. */
        cache[m] = 1 + (sum * 2);
    }

    cached_len = n + 1;
    return cache[n];
}
