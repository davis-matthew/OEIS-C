/*
 * A041326  Numerators of continued fraction convergents to sqrt(177).
 * https://oeis.org/A041326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued‑fraction convergents to sqrt(177). */
/* a(0) = 13;  a(n) = a_n * a(n‑1) + a(n‑2), where {a_n} are the continued
   fraction coefficients of sqrt(177). */

#include <stdlib.h>
#include "../utils/int_sqrt.h"

static seq_t *cf_terms = NULL;      /* continued‑fraction coefficients */
static size_t cf_len = 0;           /* number of coefficients stored */

/* Continued‑fraction state */
static idx_t m = 0, d = 1, a0 = 0, a_curr = 0;

/* Initialise coefficient array and first term */
static void init_cf_terms(void)
{
    if (cf_terms != NULL) return;

    a0 = int_sqrt((idx_t)177);         /* floor(sqrt(177)) = 13 */
    m   = 0;
    d   = 1;
    a_curr = a0;

    /* Allocate space for the first coefficient */
    cf_terms = (seq_t *)malloc(16 * sizeof(seq_t));
    cf_len   = 1;
    cf_terms[0] = (seq_t)a0;
}

/* Grow cf_terms array until it contains at least n+1 entries */
static void ensure_cf_terms(idx_t n)
{
    if (cf_len > n) return;

    init_cf_terms();

    /* Ensure enough room */
    size_t new_cap = (n + 1) * 2;
    cf_terms = (seq_t *)realloc(cf_terms, new_cap * sizeof(seq_t));

    /* Generate remaining coefficients */
    while (cf_len <= n) {
        idx_t m_next = d * a_curr - m;
        idx_t d_next = ((idx_t)177 - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        cf_terms[cf_len++] = (seq_t)a_next;

        m = m_next;
        d = d_next;
        a_curr = a_next;
    }
}

/* Main function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0) {
        init_cf_terms();
        return cf_terms[0];
    }

    ensure_cf_terms(n);

    seq_t p_nm2 = 0;   /* p_{-2} */
    seq_t p_nm1 = 1;   /* p_{-1} */
    seq_t p_n   = 0;   /* p_n */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_val = cf_terms[i];
        p_n = a_val * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }

    return p_n;
}
