/*
 * A186753  T(n, k) = Sum_{i=0, n} (Sum_{j=0, k} C(i+j,i) * C(n-i+j,n-i) *
 *          C(i+k-j,k-j) * C(n-i+k-j,n-i)).
 * https://oeis.org/A186753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A186753
 *
 *  Definition:
 *      T(n,k) := Σ_{i=0..n} Σ_{j=0..k}
 *                C(i+j,i) · C(n-i+j,n-i) · C(i+k-j,k-j) · C(n-i+k-j,n-i)
 *  Recurrence:
 *      T(n,k) - T(n-1,k) - T(n,k-1) = C(n+k,k)^2
 *  Symmetry:      T(n,k) = T(k,n)
 *  Initial:       T(0,0) = 1,  T(0,k) = k+1,  T(n,0) = n+1
 *
 *  Sequence a(n) is the table T read along its antidiagonals:
 *      n = 0  → T(0,0)
 *      n = 1  → T(0,1), n = 2 → T(1,0)
 *      n = 3  → T(0,2), n = 4 → T(1,1), n = 5 → T(2,0)
 *      etc.
 *
 *  Implementation notes:
 *      * Uses the recurrence to fill a symmetric table up to the
 *        largest antidiagonal needed for the queried n.
 *      * A modest static table size (MAX_S) is chosen to cover
 *        n up to 20 000 comfortably; this is well inside the limits
 *        required (n ≤ ~10 000).  The memory usage is modest even
 *        for 128‑bit type.
 *      * All arithmetic on binomial coefficients is done in an
 *        intermediate 128‑bit type to avoid loss of precision for
 *        small values; the result is cast back to seq_t.
 *      * No `stdbool.h` or `true/false` are used – an `int` flag
 *        suffices.
 *      * The function works correctly for any order of calls.
 */

#include "../utils/binom.h"

#define MAX_S 2000   /* covers n up to about 2 000 000 ⟶ overkill for n≤10 000 */

/* Static table and bookkeeping flag/size. */
static seq_t T[MAX_S + 1][MAX_S + 1];
static int  table_inited = 0;
static idx_t table_max_s = 0;

/* Ensure T[0..s][0..s] is filled using the recurrence. */
static void ensure_table(idx_t s_needed)
{
    if (table_inited && table_max_s >= s_needed)
        return; /* already computed enough */

    for (idx_t i = 0; i <= s_needed; ++i) {
        for (idx_t j = 0; j <= s_needed; ++j) {
            if (i == 0 && j == 0) {
                T[0][0] = (seq_t)1;
            } else if (i == 0) {
                T[0][j] = (seq_t)(j + 1);
            } else if (j == 0) {
                T[i][0] = (seq_t)(i + 1);
            } else {
                seq_t b = binom(i + j, i);        /* C(i+j,i) */
                seq_t sq = (seq_t)(((__int128)b) * b);
                T[i][j] = T[i - 1][j] + T[i][j - 1] + sq;
            }
        }
    }
    table_max_s = s_needed;
    table_inited = 1;
}

/* Return the antidiagonal index s for which
 *   s*(s+1)/2 ≤ n < (s+1)*(s+2)/2 .  Simple linear scan is fine
 *   because s stays small (≤ MAX_S). */
static idx_t find_s(idx_t n)
{
    idx_t s = 0;
    while (((s + 1) * (s + 2) / 2) <= n)
        ++s;
    return s;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    idx_t s = find_s(n);
    ensure_table(s);

    idx_t first = s * (s + 1) / 2;   /* first index of this diagonal */
    idx_t i = n - first;             /* offset along the diagonal */
    idx_t j = s - i;                 /* remaining coordinate */

    return T[i][j];
}
