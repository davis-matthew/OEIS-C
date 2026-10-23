/*
 * A157402  A partition product of Stirling_2 type [parameter k = 2] with
 *          biggest-part statistic (triangle read by rows).
 * https://oeis.org/A157402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 34, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A157402
 *
 *  This implementation builds the triangular array
 *  T(n,m) that appears in the definition:
 *
 *      T(n,0) = [n = 0]              (Iverson bracket)
 *      T(n,m) = Σ_{p≥1}^{⌊n/m⌋}
 *                ( n! / (p! (n‑p*m)!) )
 *                * ( f_m / m! )^p
 *                * T(n‑p*m , m‑1) ,
 *          for 1 ≤ m ≤ n ,
 *
 *  where f_m = ∏_{j=0}^{m‑1} (3*j‑1).
 *
 *  The sequence a(n) is obtained by reading this triangle
 *  row‑wise:  let r be such that
 *
 *      triangular(r‑1) < n ≤ triangular(r)   (triangular(r)=r(r+1)/2)
 *
 *  then
 *
 *      a(n) = T(r, n‑triangular(r‑1)) – T(r, n‑triangular(r‑1)‑1) .
 *
 *  This code pre‑computes the required rows and columns on demand
 *  and uses the same algorithm for any seq_t (fixed‑size unsigned
 *  integers or arbitrary‑precision types) and any idx_t (uint64_t).
 */

#include <stdint.h>

/* triangular number */
static inline idx_t triangular(idx_t r) { return r * (r + 1) / 2; }

/* maximum number of rows that may be needed (for n ≤ 10000, row ≤ 141) */
#define MAX_SIDE 200

/* static tables – small enough for all supported types */
static seq_t fact_arr[MAX_SIDE + 1];
static seq_t f_prod[MAX_SIDE + 1];          /* |f_m| for m = 0..MAX_SIDE */
static seq_t pow_f_abs[MAX_SIDE + 1][MAX_SIDE + 1];
static seq_t pow_fact_m[MAX_SIDE + 1][MAX_SIDE + 1];
static seq_t H[MAX_SIDE + 1][MAX_SIDE + 1];
static idx_t max_row = 0;

/* pre‑compute tables up to row target (inclusive) */
static void compute_to_row(idx_t target)
{
    if (target <= max_row)
        return;

    /* factorials and |f_m| products */
    fact_arr[0] = (seq_t)1;
    f_prod[0]  = (seq_t)1;
    for (idx_t i = 1; i <= target; ++i) {
        fact_arr[i] = fact_arr[i-1] * (seq_t)i;
        long long val = 3LL * (long long)(i - 1) - 1;          /* 3*(i-1)-1 */
        long long absval = val >= 0 ? val : -val;
        f_prod[i] = f_prod[i-1] * (seq_t)absval;
    }

    /* powers of |f_m| and (m!) */
    for (idx_t m = 1; m <= target; ++m) {
        idx_t maxp = target / m;
        pow_f_abs[m][0]  = (seq_t)1;
        pow_fact_m[m][0] = (seq_t)1;
        for (idx_t p = 1; p <= maxp; ++p) {
            pow_f_abs[m][p]  = pow_f_abs[m][p-1] * f_prod[m];
            pow_fact_m[m][p] = pow_fact_m[m][p-1] * fact_arr[m];
        }
    }

    /* initialise H */
    for (idx_t n = 0; n <= target; ++n)
        for (idx_t m = 0; m <= target; ++m)
            H[n][m] = (seq_t)0;
    for (idx_t m = 0; m <= target; ++m)
        H[0][m] = (seq_t)1;

    /* recurrence: H[n][m] = H[n][m-1] + sum_{p≥1} ... */
    for (idx_t m = 1; m <= target; ++m) {
        for (idx_t n = 0; n <= target; ++n) {
            seq_t val = H[n][m-1];
            if (n >= m) {
                idx_t maxp = n / m;
                for (idx_t p = 1; p <= maxp; ++p) {
                    seq_t denom = fact_arr[p];
                    denom = denom * fact_arr[n - (p * m)];
                    seq_t B = fact_arr[n] / denom;                     /* n!/(p! (n‑p*m)!) */
                    seq_t contrib = B * pow_f_abs[m][p] * H[n - (p * m)][m-1];
                    contrib = contrib / pow_fact_m[m][p];               /* divide by (m!)^p */
                    val += contrib;
                }
            }
            H[n][m] = val;
        }
    }

    max_row = target;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* below offset – defined as 0 */

    /* find the row that contains position n */
    idx_t row = 0;
    while (triangular(row) < n)
        ++row;

    compute_to_row(row);

    idx_t col = n - triangular(row - 1);   /* 1 ≤ col ≤ row */

    /* T(row,col) – T(row,col-1) gives the triangle entry at linear index n */
    return H[row][col] - H[row][col - 1];
}
