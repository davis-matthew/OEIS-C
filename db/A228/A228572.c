/*
 * A228572  Triangle read by rows, formed from antidiagonals of triangle
 *          A228570: T(n,k) = A034851(n-3*k, k) for n >= 0 and 0 <= k <=
 *          floor(n/4).
 * https://oeis.org/A228572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 247, u32 n <= 810, u64 n <= 989,
 *            u128 n <= 989, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A228572: a(n) is the row‑by‑row enumeration of the
 * triangle T(n,k) = A034851(n-3k, k), 0 ≤ k ≤ ⌊n/4⌋.
 * The triangle satisfies the recurrence
 *   T(0,0)=1, T(1,0)=1, T(2,0)=1, T(3,0)=1;
 *   T(n,k)=T(n-1,k)+T(n-4,k-1)
 *          -C((n+3)/2-3*(k+1)/2-1, (k+1)/2-1)   (n odd & k odd).
 * a(n) is the n-th element of the reading of this triangle.
 * The implementation keeps a memoized cache of computed rows. */

#include <stdlib.h>   /* malloc, calloc, realloc, abort */

/* Conditional cast helper: works in both C and C++ */
#if defined(__cplusplus)
# define RECAST(ptr, type) reinterpret_cast<type>(ptr)
#else
# define RECAST(ptr, type) ((type)(ptr))
#endif

/*----------------------------------------------------------*/
/* binomial coefficient for small arguments, exact division */
static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;   /* exact division */
    }
    return res;
}

/*----------------------------------------------------------*/
/* row cache: rows[r] -> array of T(r,k) for 0≤k≤⌊r/4⌋ */
static seq_t **rows     = NULL;   /* outer array of row pointers */
static size_t  row_cap  = 0;      /* capacity of rows array */
static size_t  row_cnt  = 0;      /* number of computed rows */

/* cumulative term counts: cum[r] = total terms up to and including row r */
static size_t *cum      = NULL;
static size_t  cum_cap  = 0;
static size_t  cum_cnt  = 0;      /* equals row_cnt */

/*----------------------------------------------------------*/
/* ensure capacity for rows array */
static void ensure_rows_cap(size_t needed)
{
    if (needed <= row_cap) return;
    size_t newcap = row_cap ? row_cap * 2 : 8;
    if (newcap < needed) newcap = needed;
    seq_t **tmp = RECAST(realloc(rows, newcap * sizeof(seq_t *)), seq_t **);
    if (!tmp) abort();
    rows = tmp;
    row_cap = newcap;
}

/*----------------------------------------------------------*/
/* ensure capacity for cum array */
static void ensure_cum_cap(size_t needed)
{
    if (needed <= cum_cap) return;
    size_t newcap = cum_cap ? cum_cap * 2 : 8;
    if (newcap < needed) newcap = needed;
    size_t *tmp = RECAST(realloc(cum, newcap * sizeof(size_t)), size_t *);
    if (!tmp) abort();
    cum = tmp;
    cum_cap = newcap;
}

/*----------------------------------------------------------*/
/* main sequence function */
seq_t a(idx_t n)
{
    /* extend cache until a(n) is available */
    while (row_cnt == 0 || cum[row_cnt - 1] <= n) {
        idx_t r = row_cnt;              /* new row index */
        idx_t kmax = r / 4;
        size_t rowlen = (size_t)kmax + 1;

        /* allocate zeroed row */
        seq_t *row = RECAST(calloc(rowlen, sizeof(seq_t)), seq_t *);
        if (!row) abort();

        /* base: T(r,0)=1 */
        row[0] = 1;

        /* compute other entries for r>3 */
        if (r > 3) {
            for (idx_t k = 1; k <= kmax; ++k) {
                seq_t term = 0;

                /* T(r-1,k) */
                if (k <= (r - 1) / 4) {
                    term += rows[r - 1][(size_t)k];
                }

                /* T(r-4,k-1) */
                if (r >= 4 && (k - 1) <= (r - 4) / 4) {
                    term += rows[r - 4][(size_t)(k - 1)];
                }

                /* Subtract binomial for odd n and odd k */
                if ((r & 1U) && (k & 1U)) {
                    idx_t sub_n = (r + 3) / 2 - 3 * ((k + 1) / 2) - 1;
                    idx_t sub_k = ((k + 1) / 2) - 1;
                    if (sub_n >= sub_k && sub_k >= 0) {
                        term -= binomial_small(sub_n, sub_k);
                    }
                }

                row[(size_t)k] = term;
            }
        }

        /* store the row */
        ensure_rows_cap(row_cnt + 1);
        rows[row_cnt] = row;

        /* update cumulative term count */
        size_t newcum = row_cnt == 0 ? rowlen : cum[row_cnt - 1] + rowlen;
        ensure_cum_cap(row_cnt + 1);
        cum[row_cnt] = newcum;

        ++row_cnt;
    }

    /* locate the row containing the n-th term */
    idx_t r = row_cnt - 1;
    while (r > 0 && cum[r - 1] > n) --r;

    size_t row_start = (r == 0) ? 0 : cum[r - 1];
    size_t idx_in_row = n - row_start;

    return rows[r][idx_in_row];
}
