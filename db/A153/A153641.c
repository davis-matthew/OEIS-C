/*
 * A153641  Nonzero coefficients of the Swiss-Knife polynomials for the
 *          computation of Euler, tangent, and Bernoulli numbers (triangle
 *          read by rows).
 * https://oeis.org/A153641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) = binomial(k, e) * E_{2p} * where (k,p) locate the (n‑th)
 *            non‑zero coefficient in the Swiss‑Knife * polynomial W_k(x). The
 *            exponent e is k-2p. E_m are the Euler * (secant) numbers defined
 *            by * * E_0 = 1 * sum_{i=0}^{m/2} binomial(m,2i) * E_{2i} = 0 (m
 *            even, m≥2) * * For odd indices E_m = 0....
 * Range:     i8 n <= 17, i16 n <= 33, i32 n <= 70, i64 n <= 166,
 *            i128 n <= 417, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b); */

static seq_t *euler = NULL;          /* Euler numbers, indexed by integer. */
static idx_t    euler_size = 0;      /* Number of Euler values currently stored. */

/* Ensure that Euler numbers up to 'limit' (inclusive, zero‑based) are computed. */
static void
ensure_euler(idx_t limit)
{
    if (euler == NULL) {
        euler_size = limit + 1;
        euler = (seq_t *)malloc(euler_size * sizeof(seq_t));
        /* initialise to zero */
        for (idx_t i = 0; i < euler_size; ++i) euler[i] = 0;
        euler[0] = 1;          /* E_0 = 1 */
    } else if (limit < euler_size - 1) {
        return;                /* already computed */
    }

    /* enlarge array */
    size_t new_size = limit + 1;
    seq_t *tmp = (seq_t *)realloc(euler, new_size * sizeof(seq_t));
    if (!tmp) return;          /* allocation failed: caller will crash */
    euler = tmp;
    for (idx_t i = euler_size; i < new_size; ++i) euler[i] = 0;
    euler_size = new_size;

    /* compute Euler numbers for even indices ≥2 that are not yet present */
    for (idx_t m = 2; m <= limit; m += 2) {
        /* skip if already computed */
        if (m < euler_size && euler[m] != 0) continue;

        seq_t sum = 0;
        /* sum_{i=0}^{m/2-1} binomial(m,2i) * E_{2i} */
        idx_t half = m / 2;
        for (idx_t i = 0; i < half; ++i) {
            idx_t e = 2 * i;
            seq_t coeff = binom(m, e);
            seq_t prod  = coeff * euler[e];
            sum += prod;
        }
        euler[m] = -sum;        /* recursive definition */
    }
}

/* Main function to compute the nth term of A153641 */
seq_t a(idx_t n)
{
    /* Step 1: locate row k and position p within that row */
    idx_t k = 0;
    idx_t sum = 0;                           /* terms counted so far */
    while (1) {
        idx_t t_k = (k >> 1) + 1;            /* floor(k/2)+1 */
        if (sum + t_k > n) break;           /* found the row */
        sum += t_k;
        ++k;
    }
    idx_t p = n - sum;                       /* index inside the row */

    /* Step 2: compute exponent e = k - 2p and even index d = 2p */
    idx_t e = k - 2 * p;
    idx_t d = 2 * p;                         /* even index for Euler number */

    /* Step 3: ensure Euler numbers up to required d are ready */
    ensure_euler(d);

    /* Step 4: coefficient = binomial(k, e) * E_d.  Euler is zero for odd d, but
     * d is always even here. */
    seq_t coef = binom(k, e);
    seq_t result = coef * euler[d];
    return result;
}
