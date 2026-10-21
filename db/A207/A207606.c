/*
 * A207606  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A207607; see the Formula section.
 * https://oeis.org/A207606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 96, u32 n <= 335, u64 n <= 1196,
 *            u128 n <= 4507, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Triangle T(n,k) for 0 ≤ k ≤ n‑2 is defined by
 *   T(1,0) = 1,
 *   T(2,0) = 2,
 *   T(n,k) = 2*T(n-1,k) + T(n-1,k-1) - T(n-2,k)   (k ≥ 0 and out‑of‑range terms are 0).
 *
 * The sequence a(n) is the flattened triangle : the n‑th term is the
 * coefficient indexed by the (row,column) pair that contains the n‑th
 * element in row‑major order.  In other words
 *
 *   a(1) = 1   (T(1,0))
 *   a(2) = 2   (T(2,0))
 *   a(3) = 3   (T(3,0))
 *   a(4) = 2   (T(3,1))
 *   a(5) = 4   (T(4,0))
 *   …
 *
 * The implementation memoises rows of the triangle; each row n has
 * n‑1 coefficients.  Only minimal bookkeeping is performed and the
 * function is thread‑safe for concurrent read accesses after pre‑math.
 */

#include <stdlib.h>
#include <stddef.h>

/*--- Memoisation structure ----------------------------------------------*/
static seq_t **T_rows = NULL;     /* array of pointers to rows (1‑based) */
static idx_t  T_cap   = 0;        /* allocated capacity of T_rows */
static idx_t  T_next  = 1;        /* first row index that has not yet been computed */

/*--- Ensure that rows up to 'need' (inclusive) are computed ----------------*/
static void ensure_rows(idx_t need)
{
    if (need < 1)
        return;

    /* enlarge the row‑pointer array if necessary */
    if (T_cap <= need) {
        idx_t new_cap = need + 10;            /* extra space to reduce reallocs */
        seq_t **tmp = (seq_t **)realloc(T_rows, new_cap * sizeof(seq_t *));
        T_rows = tmp;
        T_cap  = new_cap;
    }

    /* compute any missing rows from T_next up to 'need' */
    for (idx_t i = T_next; i <= need; ++i) {
        size_t len = (i <= 2) ? 1 : (size_t)(i - 1);  /* length of row i */
        T_rows[i] = (seq_t *)malloc(len * sizeof(seq_t));
        if (i == 1) {
            T_rows[i][0] = (seq_t)1;
        } else if (i == 2) {
            T_rows[i][0] = (seq_t)2;
        } else {
            /* lengths of the two preceding rows */
            size_t len_prev1 = (i - 1 <= 2) ? 1 : (size_t)(i - 2);
            size_t len_prev2 = (i - 2 <= 2) ? 1 : (size_t)(i - 3);

            for (size_t k = 0; k < len; ++k) {
                seq_t val = 0;

                if (k < len_prev1)               /* 2*T(i-1,k)          */
                    val += ((seq_t)2) * T_rows[i-1][k];

                if (k > 0 && (k - 1) < len_prev1)/* +T(i-1,k-1)         */
                    val += T_rows[i-1][k-1];

                if (k < len_prev2)               /* -T(i-2,k)            */
                    val -= T_rows[i-2][k];

                T_rows[i][k] = val;
            }
        }
    }

    T_next = need + 1;  /* next uncomputed row after 'need' */
}

/*--- Return the n-th term of the flattened triangle ----------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for indices < 1 (not used by OEIS wrapper) */

    /* Find the row 'r' that contains the n-th element.  Row 1 has 1 element,
       row 2 has 1 element, and for r ≥ 3 the number of elements is r‑1. */
    idx_t r = 1;
    idx_t cumulative = 0;
    while (1) {
        size_t len_row = (r <= 2) ? 1 : (size_t)(r - 1);
        cumulative += (idx_t)len_row;
        if (cumulative >= n)
            break;
        ++r;
    }

    size_t len_row = (r <= 2) ? 1 : (size_t)(r - 1);
    idx_t k = n - (cumulative - (idx_t)len_row) - 1;   /* zero‑based column index */

    ensure_rows(r);
    return T_rows[r][(size_t)k];
}
