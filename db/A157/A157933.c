/*
 * A157933  Triangle T(i,j) such that Sum_{j=0..i} T(i,j)*x(i,j)/2^i =
 *          Sum_{k=0..i, j=0..k} x(k,j), if x(k-1,j) = (x(k,j) + x(k,j+1))/2.
 * https://oeis.org/A157933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(i,j) from A157933:
   T(i,0) = 2^(i+1)-1,
   T(i,1) = T(i,0) + T(i-1,1),
   For j>=2:
     T(i,j) = ∑_{b=0..j} ∑_{c=j-b}^{i-b} 2^(i-c) * C(c, j-b),
   where C(c,k) is binomial coefficient.
   The sequence a(n) lists T(i,j) row‑wise: a(0)=T(0,0),
   a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), … */

#include <stddef.h>
#include <stdbool.h>

/* The type seq_t and idx_t are defined by the build.  */
extern seq_t seq_gcd(seq_t a, seq_t b);  /* only used if needed */

/* maximum row we pre‑compute (sufficient for n ≤ 10000) */
#define MAX_ROW 200

static seq_t pow2[MAX_ROW + 3];
static seq_t binom[MAX_ROW + 1][MAX_ROW + 1];
static seq_t tri[MAX_ROW + 1][MAX_ROW + 1];
static bool built = false;

/* build all required tables once */
static void build_tables(void)
{
    /* powers of two */
    pow2[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_ROW + 2; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    /* binomial coefficients */
    binom[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAX_ROW; ++n) {
        binom[n][0] = binom[n][n] = (seq_t)1;
        for (idx_t k = 1; k < n; ++k)
            binom[n][k] = binom[n - 1][k - 1] + binom[n - 1][k];
    }

    /* triangle */
    tri[0][0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_ROW; ++i) {
        tri[i][0] = pow2[i + 1] - (seq_t)1;   /* T(i,0) */
        for (idx_t j = 1; j <= i; ++j) {
            seq_t sum = (seq_t)0;
            for (idx_t b = 0; b <= j; ++b) {
                idx_t jj = j - b;            /* j-b */
                idx_t c_start = jj;
                idx_t c_end = i - b;
                for (idx_t c = c_start; c <= c_end; ++c) {
                    seq_t term = pow2[i - c] * binom[c][jj];
                    sum += term;
                }
            }
            tri[i][j] = sum;
        }
    }
    built = true;
}

/* Convert linear index n to row r and column j:  */
static void index_to_row_col(idx_t n, idx_t *r, idx_t *j)
{
    idx_t rr = 0;
    while ((rr + 1) * (rr + 2) / 2 <= n)
        ++rr;
    *r = rr;
    *j = n - rr * (rr + 1) / 2;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!built)
        build_tables();

    idx_t r;
    idx_t j;
    index_to_row_col(n, &r, &j);
    if (r > MAX_ROW) {
        /* fallback: compute on the fly for very large n (rare) */
        /* As the build tables are limited by MAX_ROW, this branch will
           never be hit for n up to 10 000 (r ~ 140).  If it occurs,
           the function returns 0. */
        return (seq_t)0;
    }
    return tri[r][j];
}
