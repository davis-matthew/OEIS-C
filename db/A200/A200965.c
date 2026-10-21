/*
 * A200965  Triangle T(n,k) = coefficient of x^n in expansion of
 *          ((1-sqrt(1-4*x))/((1-x)*2))^k = sum(n>=k, T(n,k) * x^n).
 * https://oeis.org/A200965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r is the unique integer with r(r+1)/2 >= n
 *            and c = n - (r-1)r/2. T is built by the recurrence T(r,1) =
 *            sum_{i=1}^r Catalan_{i-1}; T(r,k) = Σ_{j=1}^{r-(k-1)} S(j) *
 *            T(r-j,k-1), k≥2. here S(j)=∑_{i=1}^j Catalan_{i-1}
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A200965 – triangle T(n,k) read row‑by‑row.
   T(n,k) is the coefficient of x^n in ((1-sqrt(1-4x))/((1-x)*2))^k.
   a(n) is the triangle flattened by rows, offset 1. */

#include <stdint.h>
#include <stdbool.h>

/* maximum row needed for n up to about 10000 is 142.
   Allocate a bit more to be safe. */
#define MAX_ROW 200

static bool ready = false;         /* have we already built the triangle? */
static idx_t built_row = 0;        /* last row computed in triangle[ ][ ] */
static seq_t catalan[MAX_ROW + 1]; /* Catalan numbers C_0 … C_MAX_ROW */
static seq_t sumS[MAX_ROW + 1];    /* S(j) = Σ_{i=1}^j C_{i-1} */
static seq_t triangle[MAX_ROW + 1][MAX_ROW + 1]; /* triangle[r][k] */

/* Build the triangle up to at least row rmax. */
static void build_triangle(idx_t rmax)
{
    if (rmax <= built_row)
        return;                       /* already have enough */

    if (rmax > MAX_ROW)               /* guard against overflow */
        rmax = MAX_ROW;

    /* Catalan numbers: C_0 = 1,  C_{n+1} = C_n * (4n+2)/(n+2) */
    catalan[0] = 1;
    for (idx_t n = 0; n + 1 < rmax; ++n) {
        catalan[n + 1] = catalan[n] * (seq_t)(4 * n + 2) / (seq_t)(n + 2);
    }

    /* Sum of Catalan numbers up to index j-1 */
    sumS[0] = 0;
    for (idx_t j = 1; j <= rmax; ++j) {
        sumS[j] = sumS[j - 1] + catalan[j - 1];
    }

    /* Build triangle row by row */
    for (idx_t r = 1; r <= rmax; ++r) {
        triangle[r][1] = sumS[r];              /* first column */

        for (idx_t k = 2; k <= r; ++k) {       /* other columns */
            seq_t total = 0;
            idx_t max_j = r - (k - 1);         /* maximum j with r-j ≥ k-1 */
            for (idx_t j = 1; j <= max_j; ++j) {
                total += sumS[j] * triangle[r - j][k - 1];
            }
            triangle[r][k] = total;
        }
    }

    built_row = rmax;
    ready = true;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                           /* outside offset, trivially 0 */

    /* locate row r such that r(r+1)/2 >= n */
    idx_t r = 0;
    while (((r * (r + 1)) / 2) < n)
        ++r;

    /* column within the row */
    idx_t c = n - ((r - 1) * r) / 2;

    /* ensure triangle computed up to required row */
    build_triangle(r);

    return triangle[r][c];
}
