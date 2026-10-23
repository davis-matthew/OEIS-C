/*
 * A110169  Triangle read by rows: T(n,k) (0<=k<=n) is the number of Delannoy
 *          paths of length n that start with exactly k (1,1) steps.
 * https://oeis.org/A110169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry of the triangle T(n,k) where T(n,k) =
 *            D(n‑k) – D(n‑k‑1) for k < n, and T(n,n)=1. D(m) are the central
 *            Delannoy numbers
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard integer types. */
#include <stdint.h>

#define MAX_D 2000  /* covers all m needed for n ≤ 10000 */

/* Storage for already computed central Delannoy numbers. */
static seq_t delannoy[MAX_D];
static idx_t delannoy_max = 0;

/* Initialise D[0] (and optionally D[1]). */
static void init_delannoy(void)
{
    if (delannoy_max == 0) {
        delannoy[0] = (seq_t)1;
        if (MAX_D > 1) {
            delannoy[1] = (seq_t)3;
            delannoy_max = 1;
        }
    }
}

/* Ensure that D[0..m] are computed. */
static void ensure_delannoy(idx_t m)
{
    init_delannoy();
    if (m <= delannoy_max) return;
    if (m >= MAX_D) return;                 /* should not be needed for n≤10000 */

    for (idx_t i = delannoy_max + 1; i <= m; ++i) {
        /* Recurrence: D[i] = ((6*i-3)*D[i-1] - (i-1)*D[i-2]) / i */
        seq_t term1 = ((seq_t)(6 * i - 3)) * delannoy[i - 1];
        seq_t term2 = ((seq_t)(i - 1)) * delannoy[i - 2];
        delannoy[i] = (term1 - term2) / (seq_t)i;
    }
    delannoy_max = m;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find row r: smallest r such that (r+1)(r+2)/2 > n.
       Then cumulative up to row r-1 is r(r+1)/2 <= n.   */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Index inside the row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Special case T(n,n)=1. */
    if (k == r) return (seq_t)1;

    /* m = n - k (with n replaced by r here).  */
    idx_t m = r - k;

    ensure_delannoy(m);   /* guarantee D[m] and D[m-1] are ready */

    return delannoy[m] - delannoy[m - 1];
}
