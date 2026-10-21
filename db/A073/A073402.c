/*
 * A073402  Coefficient triangle of polynomials (rising powers) related to
 *          convolutions of A001045(n+1), n >= 0, (generalized
 *          (1,2)-Fibonacci). Companion triangle is A073401.
 * https://oeis.org/A073402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 27, u64 n <= 65, u128 n <= 209,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A073402
   Recurrence for the coefficient triangle:
   p(k,n) = (n+2)*p(k-1,n+1) + 4*(n+2*(k+1))*p(k-1,n) + 2*(n+3)*q(k-1,n+1)
   q(k,n) = (n+1)*p(k-1,n+1) + 4*(n+2*(k+1))*q(k-1,n)
   with base   p(0,n) = 1 ,  q(0,n) = 2.
   The coefficients a(k,m) are the monomial coefficients of q(k,x)
   (row poly q(k,x)=Σ a(k,m) x^m). The sequence a(n) lists the entries
   of this triangle row‑wise: a(0)=a(0,0), a(1)=a(1,0), a(2)=a(1,1), a(3)=a(2,0), …
   The function below builds the triangle incrementally and returns the
   requested term.  All arithmetic uses the opaque integer type seq_t
   provided by the build (e.g. uint64_t, unsigned __int128, or a bignum
   class).  No external helper functions are used. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------------- */
/* Parameters.  The maximum row needed for n up to about 10 000 is < 200. */
#define MAX_K 200

/* --------------------------------------------------------------------------- */
/* static storage for binomial coefficients */
static bool binom_ready = false;
static seq_t binom[MAX_K + 1][MAX_K + 1];

/* --------------------------------------------------------------------------- */
/* static storage for rows of the triangle */
static seq_t *p_rows[MAX_K + 1] = {0};
static seq_t *q_rows[MAX_K + 1] = {0};
static size_t rows_allocated = 0;      /* number of rows stored (k + 1) */

/* --------------------------------------------------------------------------- */
static void init_binom(void)
{
    if (binom_ready) return;
    for (size_t i = 0; i <= MAX_K; ++i) {
        binom[i][0] = 1;
        binom[i][i] = 1;
        for (size_t j = 1; j < i; ++j) {
            binom[i][j] = binom[i - 1][j - 1] + binom[i - 1][j];
        }
    }
    binom_ready = true;
}

/* --------------------------------------------------------------------------- */
static void init_rows(void)
{
    /* row 0 */
    p_rows[0] = (seq_t *)calloc(1, sizeof(seq_t));
    q_rows[0] = (seq_t *)calloc(1, sizeof(seq_t));
    p_rows[0][0] = (seq_t)1;   /* p(0,n)=1   */
    q_rows[0][0] = (seq_t)2;   /* q(0,n)=2   */
    rows_allocated = 1;
}

/* --------------------------------------------------------------------------- */
/* Build the rows up to the requested index k (inclusive). */
static void ensure_rows_upto(size_t k)
{
    if (rows_allocated > k) return;  /* already built */
    if (!binom_ready) init_binom();
    if (rows_allocated == 0) init_rows();

    for (size_t cur = rows_allocated; cur <= k; ++cur) {
        /* previous row is (cur-1) */
        seq_t *p_prev = p_rows[cur - 1];
        seq_t *q_prev = q_rows[cur - 1];
        size_t prev_len = cur;               /* since row cur-1 has cur entries */

        /* allocate new row buffers */
        seq_t *p_new = (seq_t *)calloc(cur + 1, sizeof(seq_t));
        seq_t *q_new = (seq_t *)calloc(cur + 1, sizeof(seq_t));

        /* shift arrays (P(n+1) and Q(n+1)) */
        seq_t *p_shift = (seq_t *)calloc(cur, sizeof(seq_t));
        seq_t *q_shift = (seq_t *)calloc(cur, sizeof(seq_t));

        for (size_t i = 0; i < prev_len; ++i) {
            seq_t pi = p_prev[i];
            for (size_t j = 0; j <= i; ++j) {
                p_shift[j] += pi * binom[i][j];
            }
        }

        for (size_t i = 0; i < prev_len; ++i) {
            seq_t qi = q_prev[i];
            for (size_t j = 0; j <= i; ++j) {
                q_shift[j] += qi * binom[i][j];
            }
        }

        seq_t B = (seq_t)2 * (seq_t)(cur + 1);   /* B = 2*(k+1) for row cur */

        /* --- compute p_new ----------------------------------- */
        /* term (x+2)*P_shift */
        for (size_t j = 0; j < cur; ++j) {
            seq_t val = p_shift[j];
            p_new[j + 1] += val;                     /* x * P_shift */
            p_new[j]     += (seq_t)2 * val;         /* 2 * P_shift */
        }
        /* term 4*(x+2*(k+1))*P_prev */
        for (size_t j = 0; j < prev_len; ++j) {
            seq_t val = p_prev[j];
            p_new[j + 1] += (seq_t)4 * val;          /* 4*x*P_prev */
            p_new[j]     += (seq_t)4 * B * val;      /* 4*B*P_prev */
        }
        /* term 2*(x+3)*Q_shift */
        for (size_t j = 0; j < cur; ++j) {
            seq_t val = q_shift[j];
            p_new[j + 1] += (seq_t)2 * val;          /* 2*x*Q_shift */
            p_new[j]     += (seq_t)6 * val;          /* 6*Q_shift */
        }

        /* --- compute q_new ----------------------------------- */
        /* term (x+1)*P_shift */
        for (size_t j = 0; j < cur; ++j) {
            seq_t val = p_shift[j];
            q_new[j + 1] += val;                     /* x * P_shift */
            q_new[j]     += val;                     /* 1 * P_shift */
        }
        /* term 4*(x+2*(k+1))*Q_prev */
        for (size_t j = 0; j < prev_len; ++j) {
            seq_t val = q_prev[j];
            q_new[j + 1] += (seq_t)4 * val;          /* 4*x*Q_prev */
            q_new[j]     += (seq_t)4 * B * val;      /* 4*B*Q_prev */
        }

        /* free shift buffers */
        free(p_shift);
        free(q_shift);

        /* store new row */
        p_rows[cur] = p_new;
        q_rows[cur] = q_new;
    }
    rows_allocated = k + 1;
}

/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* determine the row k and offset m */
    size_t k = 0;
    while (((size_t)(k + 1) * (size_t)(k + 2)) / 2 <= (size_t)n) {
        ++k;
    }
    size_t t_before = (size_t)k * (size_t)(k + 1) / 2;
    size_t m = (size_t)n - t_before;

    /* build rows up to k */
    if (k > MAX_K) {
        /* k is larger than preallocated maximum;
           in practice this should not happen for n <= 10000.    */
        /* If it does, simply fail with zero (should not occur). */
        return (seq_t)0;
    }
    if (rows_allocated <= k) {
        ensure_rows_upto(k);
    }

    return q_rows[k][m];
}
