/*
 * A161695  Number of reduced words of length n in the Weyl group A_50.
 * https://oeis.org/A161695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of * Prod_{k=1..50} (1-x^(k+1))/(1-x) *
 *            All terms beyond degree 1325 are zero. * The value of a(0) is 1,
 *            a(1)=50, a(2)=1274, …
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static const int M = 50;
static const int MAX_DEG = M*(M+3)/2;   /* 1325 */

static idx_t  init_done = 0;
static seq_t *A = NULL;   /* precomputed sequence values */

/* Precompute the sequence once */
static void init_sequence(void)
{
    int i, j, offset;
    int8_t *q_sign = NULL;        /* sign of Q(x) coefficients: -1,0,1 */
    seq_t  *comb = NULL;
    int max_comb = MAX_DEG + 49;   /* maximum index needed in comb array */

    /* Allocate and initialize the sign array for Q(x) = Prod (1 - x^(k+1) ) */
    q_sign = (int8_t*)malloc((MAX_DEG + 1) * sizeof(int8_t));
    if (!q_sign) exit(EXIT_FAILURE);
    for (i = 0; i <= MAX_DEG; ++i) q_sign[i] = 0;
    q_sign[0] = 1;  /* coefficient of x^0 is +1 */

    /* Build the signs of coefficients of Q(x) iteratively */
    for (i = 1; i <= M; ++i) {
        offset = i + 1;
        for (j = MAX_DEG; j >= offset; --j) {
            int s = q_sign[j] - q_sign[j - offset];
            /* The result is guaranteed to be in {-1,0,1} */
            q_sign[j] = (int8_t)s;
        }
    }

    /* Compute binomial coefficients C(t,49) for t = 49 .. MAX_DEG+49 */
    comb = (seq_t*)malloc((max_comb + 1) * sizeof(seq_t));
    if (!comb) exit(EXIT_FAILURE);
    comb[49] = (seq_t)1;
    for (i = 50; i <= max_comb; ++i) {
        comb[i] = comb[i - 1] * (seq_t)i;
        comb[i] = comb[i] / (seq_t)(i - 49);
    }

    /* Allocate array for the final sequence values */
    A = (seq_t*)malloc((MAX_DEG + 1) * sizeof(seq_t));
    if (!A) exit(EXIT_FAILURE);

    /* Compute a(n) = Σ_{j=0..n} q_sign[j] * C(n-j+49,49) */
    for (i = 0; i <= MAX_DEG; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (j = 0; j <= i; ++j) {
            int sign = q_sign[j];
            if (sign == 1)          /* positive contribution */
                sum_pos += comb[i - j + 49];
            else if (sign == -1)    /* negative contribution */
                sum_neg += comb[i - j + 49];
        }
        A[i] = sum_pos - sum_neg;   /* result is non‑negative */
    }

    /* Clean up temporary arrays */
    free(q_sign);
    free(comb);
    init_done = 1;
}

/* Public interface */
seq_t a(idx_t n)
{
    if (!init_done) init_sequence();
    if (n > (idx_t)MAX_DEG) {
        return (seq_t)0;
    }
    return A[(int)n];
}
