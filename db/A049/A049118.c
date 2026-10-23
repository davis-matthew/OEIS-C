/*
 * A049118  Row sums of triangle A035342 and array A134144.
 * https://oeis.org/A049118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A049118 – Row sums of triangle A035342.
    Recurrence (with a(0)=1 for convenience; a(n) in OEIS is a(n‑1)):
           a(0) = 1
           a(n+1) = ∑_{k=0}^{n} a(k) * C(n,k) * (2·(n‑k)+1)!!          */
/*  seq_t and idx_t are provided by the build system.                */

#include <stdlib.h>
#include <stddef.h>

/* Forward declarations of helper routines */
static void  ensure_cap( seq_t **arr, size_t *cap, size_t needed );
static void  extend_odd( idx_t m );

/* Cache for already computed sequence values (index 0 represents a(0)).
   The cached values correspond to the OEIS offset of 1 (so a(n) is
   stored at seq[n]).                                                     */
static seq_t *seq      = NULL;
static size_t seq_cap  = 0;          /* allocated size of seq */
static idx_t  seq_last = 0;          /* largest index computed so far */

/* Cache for odd double‑factorials (odd_df[m] = (2m+1)!!).             */
static seq_t *odd_df   = NULL;
static size_t odd_cap  = 0;          /* allocated size of odd_df */

/* Ensure that the array *arr has at least 'needed+1' elements.
   New entries (if any) are initialized to zero.                       */
static void
ensure_cap( seq_t **arr, size_t *cap, size_t needed )
{
    if (*cap > needed) return;
    size_t new_cap = (*cap > 0) ? *cap : 1;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();                /* allocation failure */
    /* Zero the newly added elements. */
    for (size_t i = *cap; i < new_cap; ++i) tmp[i] = 0;
    *arr = tmp;
    *cap = new_cap;
}

/* Extend the odd double‑factorial table up to index m (inclusive). */
static void
extend_odd( idx_t m )
{
    if (odd_cap > (size_t)m) return;

    size_t old_cap = odd_cap;
    size_t new_cap = (odd_cap > 0) ? odd_cap : 1;
    while (new_cap <= (size_t)m) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(odd_df, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();
    odd_df = tmp;

    seq_t val;
    if (old_cap == 0) {                     /* initialise first entry */
        odd_df[0] = 1;                       /* (2·0+1)!! = 1 */
        val = 1;
        old_cap = 1;
    } else {
        val = odd_df[old_cap - 1];
    }

    for (size_t i = old_cap; i < new_cap; ++i) {
        val = val * ( (seq_t)2 * (seq_t)i + 1 );
        odd_df[i] = val;
    }
    odd_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main routine required by the build system.                         */
seq_t a( idx_t n )
{
    /*  Ensure space for indices up to n. */
    ensure_cap( &seq, &seq_cap, (size_t)n );   /* we need seq[n] */

    /*  Initialise a(0) on first call. */
    if (seq_last == 0) {
        if (seq_cap < 1) seq_cap = 1;
        seq[0] = 1;
    }

    /*  Compute missing terms up to n. */
    for (idx_t m = seq_last; m < n; ++m) {
        /*  The recurrence for a(m+1).  We need odd_df up to index m. */
        extend_odd( m );

        seq_t sum   = 0;
        seq_t binom = 1;          /* C(m,0) */

        for (idx_t k = 0; k <= m; ++k) {
            seq_t term = seq[k];
            term *= odd_df[m - k];
            term *= binom;
            sum += term;

            if (k < m) {
                /* binom = C(m, k+1) = C(m, k) * (m-k)/(k+1) */
                binom = binom * ( (seq_t)(m - k) ) / ( (seq_t)(k + 1) );
            }
        }
        seq[m + 1] = sum;
    }
    seq_last = n;      /* mark the maximum computed index */

    return seq[n];
}
