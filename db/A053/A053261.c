/*
 * A053261  Coefficients of the '5th-order' mock theta function psi_1(q).
 * https://oeis.org/A053261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 26, u32 n <= 1068, u64 n <= 3697,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A053261 – Coefficients of the 5th‑order mock theta function ψ₁(q).

    a(n) = Σ_{m≥0, m(m+1)/2 ≤ n} dp[m][ n - m(m+1)/2 ]

    where dp[m][s] is the number of subsets of {1,…,m} whose elements sum to s.
    dp[0][0] = 1 and for m≥1

        dp[m][s] = dp[m-1][s] + dp[m-1][s-m]   (if s ≥ m)

    The routine builds the table incrementally; m needed for a given n is
    the largest integer with m(m+1)/2 ≤ n.  Complexity: O(m_max²) time and
    O(m_max²) memory, with m_max ≈ √(2n).  For n≈10⁴ this is trivial.   */

#include <stdlib.h>
#include <math.h>

/* --------------  dynamic programming tables -------------- */

static seq_t **dp_rows      = NULL;   /* dp_rows[m][s] – subsets of {1..m} summing to s   */
static idx_t *dp_row_len    = NULL;   /* number of entries in dp_rows[m] (max sum+1)    */
static idx_t dp_rows_cap    = 0;      /* capacity of the pointer arrays                 */
static idx_t dp_rows_len    = 0;      /* number of rows currently allocated              */

/*  Ensure dp_rows contains rows 0 … m_needed.   */
static void extend_to(idx_t m_needed)
{
    /*  First allocation – create row 0  */
    if (dp_rows_len == 0) {
        dp_rows_cap = 4;
        dp_rows      = (seq_t**)calloc(dp_rows_cap, sizeof(seq_t*));
        dp_row_len   = (idx_t *)calloc(dp_rows_cap, sizeof(idx_t));

        seq_t *row0 = (seq_t*)malloc(sizeof(seq_t));
        row0[0] = 0;
        row0[0] = 1;                     /* subset of the empty set sums to 0 */
        dp_rows[0]      = row0;
        dp_row_len[0]   = 1;
        dp_rows_len     = 1;
    }

    /*  Grow the pointer arrays if necessary  */
    if (dp_rows_len <= m_needed) {
        if (dp_rows_cap <= m_needed) {
            size_t new_cap = dp_rows_cap ? dp_rows_cap * 2 : 4;
            while (new_cap <= (size_t)m_needed) new_cap <<= 1;
            dp_rows     = (seq_t**)realloc(dp_rows,     new_cap * sizeof(seq_t*));
            dp_row_len  = (idx_t *)realloc(dp_row_len,  new_cap * sizeof(idx_t));
            dp_rows_cap = (idx_t)new_cap;
        }
        /*  Compute new rows  */
        for (idx_t cur = dp_rows_len; cur <= m_needed; ++cur) {
            idx_t cur_sum = cur * (cur + 1) / 2;      /* largest attainable sum  */
            seq_t *row = (seq_t*)malloc((size_t)(cur_sum + 1) * sizeof(seq_t));

            if (cur == 0) {                          /* row 0 already exists but keep logic clean */
                row[0] = 1;
            } else {
                idx_t prev_sum = (cur - 1) * cur / 2;
                seq_t *prev = dp_rows[cur - 1];

                /* copy contributions that do not use element cur  */
                for (idx_t s = 0; s <= prev_sum; ++s)
                    row[s] = prev[s];

                /* add subsets that use element cur  */
                for (idx_t s = cur; s <= cur_sum; ++s) {
                    seq_t add = prev[s - cur];
                    row[s] += add;
                }
            }

            dp_rows[cur]    = row;
            dp_row_len[cur] = cur_sum + 1;
        }
        dp_rows_len = m_needed + 1;
    }
}

/* --------------  public sequence routine -------------- */

seq_t a(idx_t n)
{
    /* Largest m with m(m+1)/2 ≤ n  */
    idx_t m_needed = (idx_t)((sqrt((double)(8.0L * n + 1.0L)) - 1.0L) / 2.0L);

    extend_to(m_needed);

    seq_t result = 0;
    for (idx_t m = 0; m <= m_needed; ++m) {
        idx_t tri = m * (m + 1) / 2;
        idx_t t   = n - tri;
        if (t < dp_row_len[m])        /* t is a valid index for row m  */
            result += dp_rows[m][t];
    }
    return result;
}
