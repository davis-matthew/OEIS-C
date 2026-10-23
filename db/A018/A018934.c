/*
 * A018934  From the game of Mousetrap.
 * https://oeis.org/A018934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)·subfact(n-1) + (n-2)·subfact(n-2) for n ≥ 3
 *            (subfact(k) is the number of derangements of k objects, !k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *D = NULL;    /* cache of !k (subfactorials)       */
static idx_t Dsize = (idx_t)-1;  /* last index fully computed; -1 means none yet */
static size_t Dcap  = 0;   /* allocated capacity of D             */

/* Ensure D[0..n] is computed (n ≥ 0).  The array may grow as needed. */
static void ensure_derangements(idx_t n)
{
    /* Grow the storage if required. */
    if ( (size_t)(n + 1) > Dcap ) {
        size_t new_cap = (size_t)(n + 1) * 2;
        seq_t *tmp = (seq_t*)realloc(D, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        D = tmp;
        Dcap = new_cap;
    }

    /* Initialise the first two values if the cache is empty. */
    if (Dsize == (idx_t)-1) {
        D[0] = (seq_t)1;     /* !0 = 1   */
        D[1] = (seq_t)0;     /* !1 = 0   */
        Dsize = 1;           /* last computed index */
    }

    /* Compute missing subfactorials up to n. */
    for (idx_t i = Dsize + 1; i <= n; ++i) {
        seq_t sum = D[i - 1] + D[i - 2];
        D[i] = (seq_t)(i - 1) * sum;
    }
    Dsize = n;
}

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;

    /* Make sure ! (n-1) and ! (n-2) are available. */
    ensure_derangements(n - 1);

    seq_t term1 = (seq_t)(n - 1) * D[n - 1];
    seq_t term2 = (seq_t)(n - 2) * D[n - 2];
    return term1 + term2;
}
