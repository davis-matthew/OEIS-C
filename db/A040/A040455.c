/*
 * A040455  Continued fraction for sqrt(477).
 * https://oeis.org/A040455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Continued fraction coefficients of sqrt(477).
   a(0) = 21. For n ≥ 1 the terms repeat with period length 20:
   [1, 5, 3, 1, 4, 10, 1, 2, 2, 4, 2, 2, 1, 10, 4, 1, 3, 5, 1, 42] */

#include <stdint.h>
#include <stdlib.h>

/* Radicand */
static const idx_t D = 477;

/* a0 = floor(sqrt(D)) */
static const idx_t A0i = 21;
static const seq_t A0 = (seq_t)21;

/* Period array and state */
static seq_t *period_terms = NULL;
static size_t period_len = 0;
static int initialized = 0;

/* Compute continued‑fraction period for sqrt(D) exactly once */
static void init_period(void)
{
    if (initialized) return;

    /* Allocate buffer; the period is < 2*sqrt(D) */
    size_t cap = 32;
    period_terms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!period_terms) return;     /* unlikely, but safe */

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = A0i;

    while (1) {
        /* Next step of the continued‑fraction algorithm */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (A0i + m) / d;

        /* Store the coefficient as seq_t */
        if (period_len == cap) {
            cap *= 2;
            seq_t *tmp = (seq_t *)realloc(period_terms, cap * sizeof(seq_t));
            if (!tmp) return;      /* memory failure */
            period_terms = tmp;
        }
        period_terms[period_len++] = (seq_t)a;

        /* d == 1 indicates the next term would be 2*A0i,
           so the current a is the last term of the period. */
        if (d == 1) break;
    }

    initialized = 1;
}

seq_t a(idx_t n)
{
    init_period();               /* guarantee period is ready */

    if (n == 0) return A0;       /* first term */

    /* Periodic part: a(1) ... */
    size_t idx = (size_t)((n - 1) % period_len);
    return period_terms[idx];
}
