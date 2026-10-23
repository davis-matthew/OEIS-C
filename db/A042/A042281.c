/*
 * A042281  Denominators of continued fraction convergents to sqrt(666).
 * https://oeis.org/A042281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 21, u64 n <= 41, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A042281 – Denominators of continued‑fraction convergents to sqrt(666).
 *
 * The coefficients of the continued fraction for sqrt(666) are periodic
 * with period length 16:
 *   1, 4, 5, 1, 1, 6, 1, 4, 1, 6, 1, 1, 5, 4, 1, 50
 * The recurrence for the denominators a(n) with a(-1)=0, a(0)=1 is
 *   a(n) = c[n] * a(n-1) + a(n-2),  n ≥ 1
 * where c[n] is the n‑th coefficient from the above list.
 */

#include <stdlib.h>

static const unsigned period[] = {1, 4, 5, 1, 1, 6, 1, 4, 1, 6, 1, 1, 5, 4, 1, 50};
#define PERIOD_LEN (sizeof(period) / sizeof(period[0]))

/* Memoised array of denominators. */
static seq_t *den = NULL;
static idx_t den_len = 0;  /* number of stored elements (max index + 1) */

/* Ensure that denominators up to index n are computed and cached. */
static void compute_to(idx_t n)
{
    if (den_len > n)                /* already have it */
        return;

    /* Grow the cache to size n+1 */
    idx_t new_len = n + 1;
    seq_t *tmp;

    if (den == NULL) {
        tmp = (seq_t *)malloc(sizeof(seq_t) * (size_t)new_len);
    } else {
        tmp = (seq_t *)realloc(den, sizeof(seq_t) * (size_t)new_len);
    }
    if (!tmp)
        abort();                    /* allocation failed */

    /* initialise first element if not yet present */
    if (den_len == 0) {
        tmp[0] = (seq_t)1;
        den_len = 1;
    }

    /* Compute missing entries */
    for (idx_t i = den_len; i <= n; ++i) {
        seq_t ai = period[(i - 1) % PERIOD_LEN];

        if (i == 1) {
            tmp[1] = ai * tmp[0];
        } else {
            tmp[i] = ai * tmp[i - 1] + tmp[i - 2];
        }
    }

    den = tmp;
    den_len = new_len;
}

seq_t a(idx_t n)
{
    compute_to(n);
    return den[n];
}
