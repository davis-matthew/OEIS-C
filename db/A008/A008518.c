/*
 * A008518  Triangle of Eulerian numbers with rows multiplied by 1 + x.
 * https://oeis.org/A008518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Eulerian-entry smoothed by (1 + x). * The sequence is the
 *            row‑wise listing of T(r,k) where * T(r,k) = A(r,k) + A(r,k-1) *
 *            A(r,k) are the standard Eulerian numbers with * A(0,0)=1,
 *            A(r,0)=1, A(r,r)=0 (for r>0), * A(r,k) = (k+1)·A(r-1,k) +
 *            (r-k)·A(r-1,k-1) (1≤k≤r-1). * The offset is 0 – a(0)...
 * Range:     u8 n <= 22, u16 n <= 47, u32 n <= 109, u64 n <= 240,
 *            u128 n <= 643, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Forward declaration required by the harness. */
seq_t a(idx_t n);

/* Static storage for Eulerian rows.  rows[i] holds the Eulerian
 * numbers A(i,0)..A(i,i-1) (i entries).  rows[0] is a single 1. */
static seq_t *rows[200] = {0};      /* enough for n up to ~10,000 */
static size_t rows_needed = 0;      /* highest row computed so far */

/* Ensure that rows[0..r] are available. */
static void ensure_rows(size_t r)
{
    while (rows_needed <= r) {
        size_t i = rows_needed;
        size_t len = (i == 0) ? 1 : i;          /* i+1 entries for row i */
        seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));
        if (!curr) exit(EXIT_FAILURE);          /* memory exhaustion */

        if (i == 0) {
            curr[0] = (seq_t)1;                 /* A(0,0)=1 */
        } else {
            /* Previous row is rows[i-1] with length i-1 */
            seq_t *prev = rows[i - 1];

            curr[0] = (seq_t)1;                 /* A(i,0)=1 */
            /* Middle terms, 1 <= k <= i-2 */
            for (size_t k = 1; k + 1 < i; ++k) {
                seq_t t1 = ((seq_t)(k + 1)) * prev[k];
                seq_t t2 = ((seq_t)(i - k)) * prev[k - 1];
                curr[k] = t1 + t2;
            }
            /* The last term A(i,i-1) is always 1 for i>=1 */
            if (i >= 1) {
                curr[i - 1] = (seq_t)1;
            }
        }
        rows[i] = curr;
        ++rows_needed;
    }
}

/* Main function requested by the test harness. */
seq_t a(idx_t n)
{
    /* Find row r such that cumulative entries up to row r-1
     * is <= n < cumulative entries up to row r.  Row r has
     * r+1 entries (indices 0..r).  Cumulative up to row r-1 is
     *   r*(r+1)/2.
     */
    size_t r = 0;
    while ((idx_t)(r + 1) * (idx_t)(r + 2) / 2 <= n) ++r;

    size_t k = (size_t)(n - (idx_t)r * (idx_t)(r + 1) / 2);   /* offset in row */

    /* Build Eulerian rows up to r if necessary. */
    ensure_rows(r);

    seq_t *row = rows[r];   /* row has length r for r>0; row[0] for r=0 */

    seq_t a_k   = (k <= r - 1) ? row[k] : (seq_t)0;            /* A(r,k)   */
    seq_t a_km1 = (k > 0) ? ((k - 1 <= r - 1) ? row[k - 1] : (seq_t)0) : (seq_t)0; /* A(r,k-1) */

    return a_k + a_km1;
}
