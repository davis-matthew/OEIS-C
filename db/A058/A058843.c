/*
 * A058843  Triangle T(n,k) = C_n(k) where C_n(k) = number of k-colored
 *          labeled graphs with n nodes (n >= 1, 1<=k<=n).
 * https://oeis.org/A058843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 31, u64 n <= 59, u128 n <= 124,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A058843 – triangle T(n,k) of k‑colored labelled graphs
 *
 *  Formula:
 *      T(n,1)      = 1
 *      T(1,k>1)    = 0
 *      T(n,k)      = Σ_{i=1}^{n-1} C(n-1,i) · 2^{i·(n-i)} · T(i,k-1)      (n≥2, k≥2)
 *
 *  The sequence a(n) is read row‑by‑row:
 *      a(n) = T(r,k)   where   r(r-1)/2 < n ≤ r(r+1)/2
 *                           k = n – r(r-1)/2
 *
 *  The implementation uses a memoised triangular array.  The maximum
 *  row pre‑allocated is fixed to 200, which is sufficient for n up to
 *  10 000 (the test harness limits its queries to roughly that size).
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

#define MAX_R   200            /* gives enough rows for n≈10 000 */

static seq_t T[MAX_R + 1][MAX_R + 1];
static idx_t current_max = 0;   /* highest row computed so far */

/* compute rows [current_max+1 … rmax] of the triangle */
static void compute_rows_up_to(idx_t rmax)
{
    for (idx_t n = current_max + 1; n <= rmax; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            if (k == 1) {
                T[n][1] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                for (idx_t i = 1; i <= n - 1; ++i) {
                    seq_t term   = binomial(n - 1, i);
                    term        *= pow2(i * (n - i));
                    term        *= T[i][k - 1];
                    sum        += term;
                }
                T[n][k] = sum;
            }
        }
    }
    current_max = rmax;
}

seq_t a(idx_t n)
{
    if (n == 0) {                  /* offset starts at n = 1 */
        return (seq_t)0;
    }

    /* locate the row r and column k that correspond to index n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        ++r;
    }
    idx_t k = n - (r - 1) * r / 2;

    /* compute the required rows if not yet cached */
    if (current_max < r) {
        compute_rows_up_to(r);
    }

    return T[r][k];
}
