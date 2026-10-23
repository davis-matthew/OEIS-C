/*
 * A042389  Denominators of continued fraction convergents to sqrt(721).
 * https://oeis.org/A042389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of the simple continued
 *            fraction of sqrt(721)
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 22, u64 n <= 50, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached data for memoization */
static seq_t *denoms = NULL;     /* array of denominators */
static size_t denom_cap = 0;     /* capacity of the array */
static size_t denom_len = 0;     /* current number of computed terms */

/* Constant a0 = floor(sqrt(721)) */
static const seq_t a0 = (seq_t)26;

/* Grow the cached array if needed */
static void grow_denoms(size_t needed)
{
    if (needed <= denom_cap)
        return;
    size_t new_cap = denom_cap ? denom_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(denoms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    denoms = tmp;
    denom_cap = new_cap;
}

/* Return the nth term of the sequence (a(0) = 1) */
seq_t a(idx_t n)
{
    /* Initialize on first call */
    if (denom_len == 0) {
        grow_denoms(1);
        denoms[0] = (seq_t)1;        /* q0 = 1 */
        denom_len = 1;
    }

    /* Compute terms up to index n */
    /* We keep the current state variables for the continued fraction */
    static seq_t m = 0;  /* m_n */
    static seq_t d = 1;  /* d_n */
    static seq_t a = a0; /* a_n (starts with a0) */

    while (denom_len <= (size_t)n) {
        /* Advance the continued fraction to the next term */
        seq_t m_next = d * a - m;
        seq_t d_next = ((seq_t)721 - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;   /* a_{n+1} */

        /* Compute the denominator q_{n+1} = a_{n+1}*q_n + q_{n-1} */
        seq_t q_prev1 = denoms[denom_len - 1];
        seq_t q_prev2 = (denom_len >= 2) ? denoms[denom_len - 2] : (seq_t)0;
        seq_t q_next = a_next * q_prev1 + q_prev2;

        /* Store the new term */
        grow_denoms(denom_len + 1);
        denoms[denom_len] = q_next;
        denom_len++;

        /* Update the continued‑fraction state */
        m = m_next;
        d = d_next;
        a = a_next;
    }

    return denoms[(size_t)n];
}
