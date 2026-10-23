/*
 * A145225  Triangle read by rows: T(n,k) is the number of odd permutations
 *          (of an n-set) with exactly k fixed points.
 * https://oeis.org/A145225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = C(n,k) * A000387(n-k). A000387(m) = number of odd
 *            derangements of m elements. Recurrence for odd derangements:
 *            o[0] = 0, o[1] = 0, o[m] = m * o[m-1] + ((m & 1) ? -1 : 1) * m *
 *            (m-1) / 2 (m ≥ 2). a(n) is the triangle read by rows: flatten
 *            T(n,k) row‑by‑row
 * Range:     u8 n <= 27, u16 n <= 44, u32 n <= 104, u64 n <= 252,
 *            u128 n <= 629, big any n
 * Helpers:   binom, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/ensure_cache.h"

static seq_t *odd_arr = NULL;      /* odd derangements o[m] */
static idx_t odd_cap = 0;
static idx_t odd_len = 0;          /* number of initialized entries */

/* extend odd_arr to cover indices up to n (inclusive) */
static void extend_odd(idx_t n)
{
    if (n < odd_len) return;

    if (odd_cap <= n) {
        idx_t new_cap = (odd_cap == 0) ? 16 : odd_cap * 2;
        if (new_cap <= n) new_cap = n + 16;
        ensure_cache(new_cap, &odd_arr, &odd_cap);
    }

    /* initialise from current length up to n */
    if (odd_len == 0) { odd_arr[0] = 0; odd_len = 1; }
    if (odd_len == 1) { odd_arr[1] = 0; odd_len = 2; }

    for (idx_t m = odd_len; m <= n; ++m) {
        seq_t val;
        if (m < 2) {          /* 0 and 1 are 0 */
            val = 0;
        } else {
            seq_t prod = (seq_t)m * odd_arr[m - 1];
            seq_t add  = (seq_t)m * (seq_t)(m - 1) / 2;
            if ((m & 1) == 0)   /* even m → (+1) */
                val = prod + add;
            else                /* odd  m → (-1) */
                val = prod - add;
        }
        odd_arr[m] = val;
    }
    odd_len = n + 1;
}

/* compute row (i) and column (j) indices from flattened index n */
static void row_col(idx_t n, idx_t *row, idx_t *col)
{
    idx_t i = 0;
    /* find largest i with i*(i+1)/2 <= n */
    while ((i + 1) * (i + 2) / 2 <= n) ++i;
    *row = i;
    *col = n - i * (i + 1) / 2;
}

/* OEIS sequence A145225 */
seq_t a(idx_t n)
{
    idx_t i, j;
    row_col(n, &i, &j);        /* row = i, column = j */

    idx_t m = i - j;           /* m = n - k */
    if (m >= odd_len) extend_odd(m);

    seq_t bin = binom(i, j);   /* C(i,j) */
    return bin * odd_arr[m];
}
