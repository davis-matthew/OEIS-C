/*
 * A026426  a(n) = least positive integer > a(n-1) and not a(i)*a(j) for
 *          1<=i<=j<=n, n >= 2.
 * https://oeis.org/A026426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer > a(n-1) such that a(n) is not
 *            representable as a product a(i)*a(j) for 1 <= i <= j <= n-1.
 *            Initial values: a(1)=2, a(2)=4
 * Range:     u8 n <= 112, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Binary search for a target in a sorted array of seq_t. */
static bool seq_contains(const seq_t *seq, size_t len, seq_t target)
{
    size_t lo = 0;
    size_t hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (seq[mid] == target)
            return true;
        if (seq[mid] < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Cache of computed terms.  seq_len is number of terms currently stored. */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Initialise cache with a(1)=2 and a(2)=4. */
static void ensure_base(void)
{
    if (seq_len > 0)
        return;
    seq_cap = 16;
    seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq_cache)
        return;
    seq_cache[0] = (seq_t)2;
    seq_cache[1] = (seq_t)4;
    seq_len = 2;
}

/* Grow cache if needed. */
static void grow_cache_if_needed(size_t new_len)
{
    if (new_len <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1;
    while (new_cap < new_len)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return; /* on allocation failure we keep old cache */
    seq_cache = tmp;
    seq_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined, but return 0 */

    ensure_base();

    /* If requested term already computed, just return it. */
    if (n <= seq_len)
        return seq_cache[n - 1];

    /* Compute terms up to n. */
    for (idx_t curr = seq_len + 1; curr <= n; ++curr) {
        seq_t prev = seq_cache[seq_len - 1];
        seq_t cand = prev + (seq_t)1;

        /* Find smallest candidate not in product set. */
        for (;;) {
            bool bad = false;

            /* Check every earlier term as potential factor. */
            for (size_t i = 0; i < seq_len; ++i) {
                seq_t ai = seq_cache[i];
                if (cand % ai != 0)
                    continue;
                seq_t q = cand / ai;
                if (q < ai)              /* would correspond to j < i, ignore */
                    continue;
                if (seq_contains(seq_cache, seq_len, q)) {
                    bad = true;
                    break;
                }
            }

            if (!bad) {
                /* Accept this candidate as a(curr). */
                grow_cache_if_needed(seq_len + 1);
                seq_cache[seq_len++] = cand;
                break;
            }
            ++cand;                     /* try next integer */
        }
    }

    return seq_cache[n - 1];
}
