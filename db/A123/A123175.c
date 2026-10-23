/*
 * A123175  Table (read by antidiagonals) where t(0,0)=1, t(m,n) = number of
 *          terms above and to the left of t(m,n) (i.e., number of t(k,j)'s,
 *          where 0 <= k <= m, 0 <= j <= n, excluding the t(m,n) case itself)
 *          which are coprime to (m+n).
 * https://oeis.org/A123175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t(m,n) read by antidiagonals, where * t(0,0) = 1 and for
 *            m + n > 0, * t(m,n) = #{ (k,j) | 0 ≤ k ≤ m , 0 ≤ j ≤ n , (k,j) ≠
 *            (m,n), * gcd( t(k,j) , m+n ) = 1 }. * The sequence is obtained
 *            by listing the table entries * (m,n) in antidiagonal order: *
 *            0≤m≤s, n = s-m for each s = 0,1,2,…
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation keeps a growing cache of all previously
 *  computed table values together with their coordinates.
 *  Each new element scans all earlier entries, checks whether
 *  they lie inside the rectangle defined by the current
 *  (m,n), and whether their value is coprime to the current
 *  sum s = m+n.  The count of such entries is stored as the
 *  next sequence term.  Caching allows arbitrary‑order
 *  calls in sub‑quadratic (O(n²)) time for n up to about 10⁴. */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache of all computed table entries */
    static seq_t  *vals   = NULL;   /* t‑values */
    static idx_t  *ms     = NULL;   /* row   indices (m) */
    static idx_t  *ns     = NULL;   /* column indices (n) */
    static size_t  cap    = 0;      /* allocated capacity */
    static size_t  len    = 0;      /* current number of stored terms */

    /* where we are in the generation process */
    static idx_t   next_diag   = 0; /* current diagonal sum s */
    static idx_t   pos_on_diag = 0; /* next m to process on current diagonal */

    /* allocate initial storage */
    if (!vals) {
        cap = 16;
        vals = (seq_t *)malloc(cap * sizeof(seq_t));
        ms   = (idx_t *)malloc(cap * sizeof(idx_t));
        ns   = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!vals || !ms || !ns) abort();
    }

    /* generate entries until we have indexed up to n */
    while (len <= n) {
        idx_t s = next_diag; /* current diagonal sum */

        /* process all remaining positions on this diagonal */
        for (idx_t m = pos_on_diag; m <= s; ++m) {
            idx_t n_coord = s - m;

            seq_t cnt;
            if (s == 0 && m == 0) {
                /* the sole base case */
                cnt = 1;
            } else {
                cnt = 0;
                /* count qualifying previous entries */
                for (size_t k = 0; k < len; ++k) {
                    if (ms[k] <= m && ns[k] <= n_coord) {
                        if (gcd(vals[k], (seq_t)s) == 1) ++cnt;
                    }
                }
            }

            /* enlarge storage if necessary */
            if (len >= cap) {
                size_t new_cap = cap ? cap << 1 : 16;
                seq_t *new_vals = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
                idx_t *new_ms   = (idx_t *)realloc(ms,   new_cap * sizeof(idx_t));
                idx_t *new_ns   = (idx_t *)realloc(ns,   new_cap * sizeof(idx_t));
                if (!new_vals || !new_ms || !new_ns) abort();
                vals = new_vals; ms = new_ms; ns = new_ns; cap = new_cap;
            }

            /* store the new entry */
            vals[len] = cnt;
            ms[len]   = m;
            ns[len]   = n_coord;
            ++len;

            if (len > n) {
                /* finished computing the requested term */
                /* remember where we left off for future calls */
                pos_on_diag = m + 1;
                if (pos_on_diag > s) {
                    next_diag = s + 1;
                    pos_on_diag = 0;
                }
                return vals[n];
            }
        }

        /* finished entire diagonal */
        pos_on_diag = 0;
        ++next_diag;
    }

    /* this point is never reached */
    return vals[n];
}
