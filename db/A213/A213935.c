/*
 * A213935  Triangle with entry a(n,m) giving the total number of necklaces of
 *          n beads (C_n symmetry) with n colors available for each bead, but
 *          only m distinct colors present, with m from {1, 2, ..., n} and n
 *          >= 1.
 * https://oeis.org/A213935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   a(n) is the n-th entry of the triangle a(r,m) defined by a(r,m)
 *            = (1/r) * sum_{g|r} φ(r/g) * P(r,m) * S(g,m), where P(r,m) =
 *            r!/(r-m)! and S(g,m) are Stirling numbers of the second kind.
 *            The flattened sequence is read by rows: a(1)=1; a(2)=2, a(3)=1;
 *            a(4)=3, a(5)=6, a(6)=2; ... etc
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation is fully self‑contained and works for both the
   fixed‑size integer builds and the arbitrary‑precision build. */

#include <stdlib.h>
#include <stdint.h>

/* ---------  helpers for Stirling numbers  --------------------------------- */

/* dynamic array of rows of Stirling numbers of second kind
   S_rows[g] points to an array of size g+1,  index 0..g */
static seq_t **S_rows = NULL;    /* array of ptrs to rows */
static idx_t  S_rows_cnt = 0;    /* number of rows currently allocated (highest index +1) */

/* ensure that rows 0..maxR of S are allocated and computed */
static void ensure_S_rows(idx_t maxR)
{
    if (S_rows_cnt > maxR) return;
    if (!S_rows) {
        S_rows = (seq_t**)malloc(sizeof(seq_t*) * (maxR + 1));
    } else {
        S_rows = (seq_t**)realloc(S_rows, sizeof(seq_t*) * (maxR + 1));
    }
    for (idx_t g = S_rows_cnt; g <= maxR; ++g) {
        seq_t *row = (seq_t*)malloc(sizeof(seq_t) * (g + 1));
        if (g == 0) {
            row[0] = (seq_t)1;
        } else {
            row[0] = (seq_t)0;
            for (idx_t k = 1; k <= g; ++k) {
                seq_t term = S_rows[g-1][k-1];
                if (k <= g-1) term += (seq_t)k * S_rows[g-1][k];
                row[k] = term;
            }
        }
        S_rows[g] = row;
    }
    S_rows_cnt = maxR + 1;
}

/* ---------  helpers for phi(r)  ------------------------------------------- */

/* small function: compute Euler's totient φ(n) for n>=1, result in seq_t  */
static seq_t phi_val(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t result = (seq_t)n;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result = result / (seq_t)p * (p - 1);
        }
    }
    if (temp > 1) {
        result = result / (seq_t)temp * (temp - 1);
    }
    return result;
}

/* ---------  cache for rows of the triangle  -------------------------------- */

static seq_t **a_rows = NULL;    /* a_rows[r] points to array size r+1, indices 1..r   */
static idx_t  a_rows_cnt = 0;    /* number of rows currently allocated (max index +1) */

/* compute all entries for row r (1‑based) and store them */
static void compute_row(idx_t r)
{
    if (a_rows_cnt > r) return;          /* already computed */
    if (!a_rows) {
        a_rows = (seq_t**)malloc(sizeof(seq_t*) * (r + 1));
    } else {
        a_rows = (seq_t**)realloc(a_rows, sizeof(seq_t*) * (r + 1));
    }
    for (idx_t rr = a_rows_cnt; rr <= r; ++rr) {
        seq_t *row = (seq_t*)malloc(sizeof(seq_t) * (rr + 1));
        /* Compute Stirling numbers up to rr to use later */
        ensure_S_rows(rr);

        seq_t P = (seq_t)1;          /* P = r!/(r-m)! , start with m=0 */
        for (idx_t m = 1; m <= rr; ++m) {
            P = P * (seq_t)(rr - m + 1);      /* update P for this m */

            seq_t sum = (seq_t)0;
            /* iterate over divisors g of rr */
            for (idx_t d = 1; (idx_t)d * d <= rr; ++d) {
                if (rr % d == 0) {
                    idx_t g1 = d;
                    if (m <= g1) {
                        seq_t term = phi_val(rr / g1);
                        term = term * S_rows[g1][m];
                        term = term * P;
                        sum += term;
                    }
                    idx_t g2 = rr / d;
                    if (g2 != g1 && m <= g2) {
                        seq_t term = phi_val(rr / g2);
                        term = term * S_rows[g2][m];
                        term = term * P;
                        sum += term;
                    }
                }
            }
            row[m] = sum / (seq_t)rr;    /* integer division */
        }
        a_rows[rr] = row;
    }
    a_rows_cnt = r + 1;
}

/* ---------  public function  ------------------------------------------------ */

seq_t a(idx_t n_index)
{
    /* find row r such that triangular number T_{r-1} < n_index <= T_r */
    idx_t r = 0;
    while ( ( (r + 1) * (r + 2) / 2 ) < n_index ) ++r;
    ++r; /* now r is the row number (1‑based) */

    idx_t prev_t = (r - 1) * r / 2;
    idx_t c = n_index - prev_t;           /* column index in the row   (1‑based) */

    compute_row(r);                       /* ensure row r is ready      */

    return a_rows[r][c];
}
