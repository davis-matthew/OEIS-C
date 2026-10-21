/*
 * A209424  Triangle defined by g.f.: A(x,y) = exp( Sum_{n>=1} x^n/n *
 *          Sum_{k=0..n} binomial(n, k)^n * y^k ), as read by rows.
 * https://oeis.org/A209424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 57, u128 n <= 95,
 *            big any n
 * Helpers:   binom_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A209424
    Triangle defined by the generating function
        A(x,y) = exp( Σ_{m≥1} x^m/m * Σ_{k=0..m} binomial(m,k)^m y^k ),
    read row‑wise, offset 0.
    The value a(n) is the nth element of that row‑wise flattened triangle.

    Recurrence for the polynomial coefficients a_{r,c}  (row r, column c):
        a_{0,0} = 1
        a_{r,c} = (1/r) · Σ_{m=1..r} Σ_{l=0..c} a_{r-m,c-l} · binomial(m,l)^m
                  (treat a_{i,j}=0 if j>i or i<0).
*/

#include <stddef.h>
#include <stdint.h>
#include "../utils/binom_func.h"

/* --------------------------------------------------------------------- */

/* Helper: exponentiation by squaring (uses idx_t exponent) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* --------------------------------------------------------------------- */
/* static storage (enough for rows up to 240: 48048 terms)                */
#define MAX_R 256                     /* maximum row index */
/* triangle[r][c] holds a_{r,c} for 0 <= c <= r < MAX_R                   */
static seq_t triangle[MAX_R][MAX_R] = {{0}};
/* binom_pow[r][k] = binomial(r,k)^r                                        */
static seq_t binom_pow[MAX_R][MAX_R] = {{0}};
static idx_t computed_rows = 0;        /* highest row that has been computed - 1 (row 0 pending) */
static int initialized = 0;           /* flag whether row 0 has been set up */

/* --------------------------------------------------------------------- */
/* Initializes row 0.  Called on first use. */
static void init_rows(void)
{
    triangle[0][0] = (seq_t)1;
    binom_pow[0][0] = (seq_t)1;
    computed_rows = 0;
    initialized = 1;
}

/* --------------------------------------------------------------------- */
/* Ensures that rows 0..r_needed are computed.                           */
static void ensure_rows(idx_t r_needed)
{
    if (!initialized)
        init_rows();

    if (r_needed <= (idx_t)computed_rows)
        return;                 /* already computed */

    /* Compute binomial powers for new rows */
    for (idx_t n = (computed_rows + 1); n <= r_needed; ++n)
    {
        for (idx_t k = 0; k <= n; ++k)
        {
            seq_t bin = binom_func(n, k);
            binom_pow[n][k] = pow_seq(bin, n);
        }
    }

    /* Compute triangle coefficients using the recurrence */
    for (idx_t n = (computed_rows + 1); n <= r_needed; ++n)
    {
        seq_t temp[MAX_R];
        for (idx_t t = 0; t <= n; ++t) temp[t] = (seq_t)0;

        for (idx_t m = 1; m <= n; ++m)
        {
            idx_t rem = n - m;
            for (idx_t k = 0; k <= rem; ++k)
            {
                seq_t a_nm = triangle[rem][k];
                if (a_nm == (seq_t)0) continue;
                for (idx_t l = 0; l <= m; ++l)
                {
                    temp[k + l] += a_nm * binom_pow[m][l];
                }
            }
        }

        seq_t den = (seq_t)n;    /* n is >=1 here */
        for (idx_t t = 0; t <= n; ++t)
            triangle[n][t] = temp[t] / den;
    }

    computed_rows = r_needed;
}

/* --------------------------------------------------------------------- */
/* Public function: returns a(n) for the flattened triangle.             */
seq_t a(idx_t n)
{
    /* Determine which row contains index n */
    idx_t r = 0;
    while (( (r + 1) * (r + 2) / 2 ) <= n) ++r;

    ensure_rows(r);

    /* Column within that row */
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;            /* 0 <= c <= r */

    return triangle[r][c];
}
