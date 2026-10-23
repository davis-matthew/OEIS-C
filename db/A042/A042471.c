/*
 * A042471  Denominators of continued fraction convergents to sqrt(763).
 * https://oeis.org/A042471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042471: Denominators of continued fraction convergents to sqrt(763)
   Recurrence:  a(n) = 1439449202*a(n-22) - a(n-44)  for n>43
   Initial values a(0) … a(43) are obtained from the continued‑fraction
   expansion of sqrt(763).  The code caches computed terms so that calls
   in any order are fast. */
#include <stdlib.h>

/* Cached denominators */
static seq_t *den = NULL;
static idx_t den_len = 0;

/* Multiplicative constant in the recurrence */
static const seq_t K = (seq_t)1439449202;

/* Precompute a(0) … a(43) from the continued fraction of sqrt(763) */
static void init_denominators(void)
{
    /* There are 44 terms (indices 0 … 43) before the recurrence starts. */
    den = (seq_t *)calloc(44, sizeof(seq_t));
    if (!den) return;         /* allocation failed – unlikely in tests */

    /* Continued‑fraction parameters */
    int m = 0, d = 1;
    const int a0 = 27;         /* floor(sqrt(763)) */
    int a = a0;                /* current coefficient a_i */

    /* Recurrence for convergents:  q_{-2}=1, q_{-1}=0 */
    seq_t q_minus2 = 1;
    seq_t q_minus1 = 0;

    /* Compute q_0 … q_43 */
    for (idx_t i = 0; i <= 43; ++i) {
        seq_t q = (seq_t)a * q_minus1 + q_minus2;
        den[i] = q;                /* store q_i */
        q_minus2 = q_minus1;
        q_minus1 = q;

        /* Next continued‑fraction coefficient (for i+1) */
        int m_next   = d * a - m;
        int d_next   = (763 - m_next * m_next) / d; /* integer division exact */
        int a_next   = (a0 + m_next) / d_next;
        m = m_next;
        d = d_next;
        a = a_next;
    }

    den_len = 44;                 /* indices 0 … 43 are ready */
}

/* Main access routine */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (den == NULL) init_denominators();

    /* Return directly if already computed */
    if (n < den_len) return den[n];

    /* Extend cache using the linear recurrence */
    for (idx_t i = den_len; i <= n; ++i) {
        seq_t val = K * den[i - 22] - den[i - 44];

        /* Reallocate one more element */
        den = (seq_t *)realloc(den, (i + 1) * sizeof(seq_t));
        den[i] = val;
    }
    den_len = n + 1;
    return den[n];
}
