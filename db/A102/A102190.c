/*
 * A102190  Irregular triangle read by rows: coefficients of cycle index
 *          polynomial for the cyclic group C_n, Z(C_n,x), multiplied by n.
 * https://oeis.org/A102190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = φ(k) where k is the divisor of the row containing the
 *            n-th coefficient. Each row n lists φ(d) for the divisors d of n
 *            in increasing order, and the rows are concatenated in order
 * Range:     u8 n <= 1467, u16 n <= 7069, u32 n <= 7069, u64 n <= 7069,
 *            u128 n <= 7069, big any n
 * Helpers:   count_divisors, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/phi.h"
#include "../utils/count_divisors.h"

/* static state for caching cumulative divisor counts   */
static idx_t *cum = NULL;      /* cum[r] = total terms up to row r */
static idx_t rows_filled = 0;  /* number of rows currently computed */
static size_t cum_alloc = 0;   /* allocated size of cum array (elements) */

/* helper: ensure cum array has room for at least 'need' indices (0..need) */
static void ensure_cum_capacity(idx_t need)
{
    if (cum_alloc <= (size_t)need) {
        size_t new_alloc = cum_alloc == 0 ? 16 : cum_alloc * 2;
        if (new_alloc <= (size_t)need)
            new_alloc = (size_t)need + 1;
        idx_t *tmp = (idx_t *)realloc(cum, new_alloc * sizeof(idx_t));
        cum = tmp;
        cum_alloc = new_alloc;
        if (cum == NULL) abort();  /* out of memory */
    }
}

/* main function */
seq_t a(idx_t n)
{
    /* initialise cum array lazily */
    if (cum == NULL) {
        cum = (idx_t *)malloc(sizeof(idx_t));
        if (cum == NULL) abort();
        cum[0] = 0;          /* no terms before row 1 */
        cum_alloc = 1;
        rows_filled = 0;
    }

    /* extend cumulative data until it covers index n */
    while (cum[rows_filled] < n) {
        idx_t row = rows_filled + 1;                    /* row number to add */
        idx_t tau = (idx_t)count_divisors(row);          /* number of divisors of 'row' */

        ensure_cum_capacity(rows_filled + 1);            /* room for cum[row] */
        cum[row] = cum[rows_filled] + tau;
        rows_filled = row;
    }

    /* locate the row containing index n (binary search) */
    idx_t lo = 1, hi = rows_filled;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo;                           /* the required row */

    /* position within the row (0‑based) */
    idx_t offset = n - cum[row - 1] - 1;       /* offset into this row */

    /* compute all divisors of 'row' in increasing order */
    idx_t tau = cum[row] - cum[row - 1];       /* number of divisors of this row */
    idx_t *div = (idx_t *)malloc(tau * sizeof(idx_t));
    if (div == NULL) abort();
    idx_t *rev = (idx_t *)malloc(tau * sizeof(idx_t));
    if (rev == NULL) abort();

    idx_t small_cnt = 0, rev_cnt = 0;
    for (idx_t d = 1; d * d <= row; ++d) {
        if (row % d == 0) {
            div[small_cnt++] = d;
            if (d * d != row) {
                rev[rev_cnt++] = row / d;
            }
        }
    }
    /* append reversed large divisors to get ascending order */
    for (idx_t i = rev_cnt; i > 0; --i) {
        div[small_cnt++] = rev[i - 1];
    }
    free(rev);

    /* fetch the divisor at the desired offset */
    idx_t divisor = div[offset];
    free(div);

    /* return its totient */
    return phi(divisor);
}
