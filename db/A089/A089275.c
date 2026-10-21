/*
 * A089275  Coefficient triangle of polynomials used for numerator of g.f.s
 *          for column sequences of array A078739.
 * https://oeis.org/A089275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) computed from the coefficient data of the polynomials * *
 *            Let a(k,m), b(k,m) be defined by the recurrences on the OEIS
 *            entry: * * a(1,0) = 1, b(1,0) = 1 * * a(k,m) = a(k-1,m) * -
 *            (2k-1)(2k-2) a(k-1,m-1) * + 4k(2k-1)(k-1) b(k-1,m-1) * * b(k,m)
 *            = 1/(k+1) * * [ 6k(2k-1)(k-1) b(k-1,m-1) * + (k-1...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

#define MAX_K 300   /* sufficient for n up to > 10000 (k≈141) */

static seq_t a_prev[MAX_K];
static seq_t b_prev[MAX_K];
static seq_t a_cur[MAX_K];
static seq_t b_cur[MAX_K];
static idx_t cur_k = 0;     /* highest k already computed */

/* Compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for offset 0, though never used */

    /* Find k such that k*(k+1)/2 ≥ n */
    idx_t k = 1;
    while (k * (k + 1) / 2 < n) ++k;

    /* Column index within row k */
    idx_t tri_prev = (k - 1) * k / 2;
    idx_t m = n - tri_prev - 1;   /* 0‑based */

    /* Initialise first row if not done yet */
    if (cur_k == 0) {
        a_prev[0] = 1;
        b_prev[0] = 1;
        cur_k = 1;
    }

    /* Extend triangles up to desired k */
    while (cur_k < k) {
        idx_t nextk = cur_k + 1;      /* this will become new row index */
        /* ----- compute a(nextk, m) ----- */
        for (idx_t j = 0; j < nextk; ++j) {
            if (j == 0) {
                /* a(nextk,0) = a(cur_k,0) */
                a_cur[j] = a_prev[0];
            } else {
                /* a(prev,m) and b(prev,m-1) are 0 if indices out of range */
                seq_t term1 = a_prev[j];           /* a(prev,m)   */
                seq_t term2 = (seq_t)(2 * nextk - 1) *
                              (seq_t)(2 * nextk - 2) * a_prev[j - 1];
                seq_t term3 = (seq_t)4 * (seq_t)nextk *
                              (seq_t)(2 * nextk - 1) *
                              (seq_t)(nextk - 1) * b_prev[j - 1];
                seq_t sum = term1 + term3;
                a_cur[j] = (sum >= term2) ? (sum - term2) : 0;
            }
        }

        /* ----- compute b(nextk, m) ----- */
        for (idx_t j = 0; j < nextk; ++j) {
            seq_t num = 0;
            /* 6k(2k-1)(k-1) b(prev,m-1) */
            if (j > 0)
                num += (seq_t)6 * (seq_t)nextk *
                       (seq_t)(2 * nextk - 1) *
                       (seq_t)(nextk - 1) * b_prev[j - 1];
            /* (k-1) b(prev,m) */
            if (j < cur_k)
                num += (seq_t)(nextk - 1) * b_prev[j];
            /* 2 a(prev,m) */
            if (j < cur_k)
                num += (seq_t)2 * a_prev[j];
            /* -4(2k-1)(k-1) a(prev,m-1) */
            if (j > 0)
                num -= (seq_t)4 * (seq_t)(2 * nextk - 1) *
                       (seq_t)(nextk - 1) * a_prev[j - 1];
            /* division by (k+1) */
            b_cur[j] = num / (nextk + 1);
        }

        /* Move current row to previous for next iteration */
        for (idx_t j = 0; j < nextk; ++j) {
            a_prev[j] = a_cur[j];
            b_prev[j] = b_cur[j];
        }
        cur_k = nextk;
    }

    /* Return the requested coefficient in row k */
    return a_prev[m];
}
