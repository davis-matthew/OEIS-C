/*
 * A047807  Duplicate of A008700.
 * https://oeis.org/A047807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2/3 E_4^3 + 1/3 E_6^2
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 4, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* |
 *  E4(z) = 1 + 240 Σ σ3(m) q^m
 *  E6(z) = 1 - 504 Σ σ5(m) q^m
 *  a(n) = (2*A(n)+B(n))/3
 *  where
 *      A(n) = 3·240·σ3(n) + 3·240²·(σ3*σ3)(n) + 240³·(σ3*σ3*σ3)(n)
 *      B(n) = -1008·σ5(n) + 504²·(σ5*σ5)(n)
 *  (σk*σk)(n) and (σk*σk*σk)(n) are Dirichlet convolutions of σk.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

static idx_t  global_max_n = 0;
static seq_t *sigma3      = NULL;
static seq_t *sigma5      = NULL;
static seq_t *conv2_3     = NULL;   /* σ3 * σ3 */
static seq_t *conv3_3     = NULL;   /* σ3 * σ3 * σ3 */
static seq_t *conv2_5     = NULL;   /* σ5 * σ5 */

/* Allocate or resize all auxiliary tables up to N */
static void ensure_tables(idx_t N)
{
    if (N <= global_max_n)
        return;

    /* free old tables */
    if (sigma3)   free(sigma3);
    if (sigma5)   free(sigma5);
    if (conv2_3)  free(conv2_3);
    if (conv3_3)  free(conv3_3);
    if (conv2_5)  free(conv2_5);

    global_max_n = N;
    size_t sz = (size_t)(N + 1);

    sigma3  = (seq_t *)calloc(sz, sizeof(seq_t));
    sigma5  = (seq_t *)calloc(sz, sizeof(seq_t));
    conv2_3 = (seq_t *)calloc(sz, sizeof(seq_t));
    conv3_3 = (seq_t *)calloc(sz, sizeof(seq_t));
    conv2_5 = (seq_t *)calloc(sz, sizeof(seq_t));

    /* ----- divisor sums σ3, σ5 --------------------------------- */
    for (idx_t d = 1; d <= N; ++d) {
        seq_t d3 = (seq_t)d * d * d;          /* d^3  */
        seq_t d5 = d3 * (seq_t)d * (seq_t)d;   /* d^5  */
        for (idx_t m = d; m <= N; m += d) {
            sigma3[m] += d3;
            sigma5[m] += d5;
        }
    }

    /* ----- convolution σ3 * σ3 --------------------------------- */
    for (idx_t i = 1; i <= N; ++i) {
        seq_t si = sigma3[i];
        for (idx_t j = 1; i + j <= N; ++j) {
            conv2_3[i + j] += si * sigma3[j];
        }
    }

    /* ----- convolution σ5 * σ5 --------------------------------- */
    for (idx_t i = 1; i <= N; ++i) {
        seq_t si = sigma5[i];
        for (idx_t j = 1; i + j <= N; ++j) {
            conv2_5[i + j] += si * sigma5[j];
        }
    }

    /* ----- convolution σ3 * σ3 * σ3 ---------------------------- */
    for (idx_t i = 1; i <= N; ++i) {
        seq_t si = sigma3[i];
        for (idx_t j = 1; i + j <= N; ++j) {
            conv3_3[i + j] += si * conv2_3[j];
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_tables(n);

    seq_t s3 = sigma3[n];
    seq_t s5 = sigma5[n];

    seq_t c240   = 240;
    seq_t c240sq = c240 * c240;          /* 240^2 */
    seq_t c240cub = c240sq * c240;       /* 240^3 */
    seq_t c504sq = 504 * 504;            /* 504^2 */

    seq_t A = 3 * c240 * s3
            + 3 * c240sq * conv2_3[n]
            + c240cub * conv3_3[n];

    seq_t B = -((seq_t)1008) * s5
            + c504sq * conv2_5[n];

    seq_t result = (2 * A + B) / (seq_t)3;
    return result;
}
