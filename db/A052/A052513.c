/*
 * A052513  Number of labeled trees of height at most 3.
 * https://oeis.org/A052513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * E_{n-1} where E is the integer EGF-coefficients of *
 *            exp( exp( x*exp(x) ) ), derived from the EGF * U(x) = x * exp( x
 *            * exp( x * exp(x) ) ) * * The recursive computation uses only
 *            integer arithmetic and the combinatorial * binomial coefficients
 *            binom(m-1,k-1) computed multiplicatively
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *B = NULL;   /* B[m] = m! * coeff of x^m/m! in exp(x*exp(x))   */
static seq_t *E = NULL;   /* E[m] = m! * coeff of x^m/m! in exp( x*exp(x*exp(x)) ) */
static size_t cap = 0;    /* current allocated size of B and E */

/* Ensure arrays B and E have space for indices 0 .. target inclusive */
static void ensure_cap(idx_t target)
{
    if ((size_t)target < cap) return;   /* already big enough */
    size_t new_cap = (size_t)(target + 1) * 2;  /* double to reduce reallocs */
    seq_t *newB = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
    seq_t *newE = (seq_t *)realloc(E, new_cap * sizeof(seq_t));
    if (!newB || !newE) return;  /* allocation failure, program will terminate */
    B = newB;
    E = newE;
    cap = new_cap;
}

/* Compute sequence value a(n) for n >= 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t target = (size_t)(n - 1);
    ensure_cap((idx_t)target);
    if ((size_t)target >= cap) return (seq_t)0; /* safety guard */

    /* B[0] must be 1 */
    if ((size_t)0 >= cap) return (seq_t)0; /* safety */
    B[0] = (seq_t)1;

    /* Compute B[m] for m = 1 .. target */
    for (size_t m = 1; m <= target; ++m) {
        seq_t sum = 0;
        seq_t binom = 1; /* binom(m-1, 0) */
        for (size_t k = 1; k <= m; ++k) {
            /* term = binom * k * B[m-k] */
            seq_t term = binom * (seq_t)k * B[m - k];
            sum += term;
            if (k < m) {
                /* update binom to binom(m-1, k) */
                seq_t num = (seq_t)(m - k);
                seq_t den = (seq_t)k;
                binom = (binom * num) / den;
            }
        }
        B[m] = sum;
    }

    /* Compute D[k] = k * B[k-1] for k = 1 .. target */
    /* Rather than store D separately, compute on the fly in E-loop */

    /* E[0] = 1 */
    E[0] = (seq_t)1;

    /* Compute E[m] for m = 1 .. target */
    for (size_t m = 1; m <= target; ++m) {
        seq_t sum = 0;
        seq_t binom = 1; /* binom(m-1, 0) */
        for (size_t k = 1; k <= m; ++k) {
            /* D_k = k * B[k-1] */
            seq_t Dk = (seq_t)k * B[k - 1];
            seq_t term = binom * Dk * E[m - k];
            sum += term;
            if (k < m) {
                /* update binom to binom(m-1, k) */
                seq_t num = (seq_t)(m - k);
                seq_t den = (seq_t)k;
                binom = (binom * num) / den;
            }
        }
        E[m] = sum;
    }

    /* Result: a(n) = n * E_{n-1} */
    seq_t result = (seq_t)n * E[target];
    return result;
}
