/*
 * A151478  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0,0), ending on the vertical axis and consisting of n steps taken
 *          from {(-1, -1), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 1)}.
 * https://oeis.org/A151478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = M(n) * binomial(n, floor(n/2)) * where M(n) are the
 *            Motzkin numbers defined by * M(0) = 1, M(1) = 1, * (n+2) * M(n)
 *            = (2n+1) * M(n-1) + (3n-3) * M(n-2) for n ≥ 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *motzkin = NULL;
static idx_t motzkin_cap = 0;

/* Ensure that the Motzkin array contains values up to index n   */
static void ensure_motzkin(idx_t n)
{
    if (n + 1 <= motzkin_cap)
        return;

    idx_t old_cap = motzkin_cap;
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(motzkin, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                         /* allocation failure */
    motzkin = tmp;

    if (old_cap == 0) {
        motzkin[0] = (seq_t)1;
        if (new_cap > 1)
            motzkin[1] = (seq_t)1;
    } else if (old_cap == 1 && new_cap > 1) {
        if (motzkin[1] == (seq_t)0)
            motzkin[1] = (seq_t)1;
    }

    /* compute new entries using the recurrence */
    idx_t start = (old_cap > 1 ? old_cap : 2);
    for (idx_t i = start; i < new_cap; ++i) {
        seq_t t1 = ((seq_t)(2*i + 1)) * motzkin[i-1];
        seq_t t2 = ((seq_t)(3*i - 3)) * motzkin[i-2];
        seq_t numer = t1 + t2;
        seq_t denom = (seq_t)(i + 2);
        motzkin[i] = numer / denom;          /* division exact */
    }

    motzkin_cap = new_cap;
}

/* Public function called by the test harness                     */
seq_t a(idx_t n)
{
    ensure_motzkin(n);
    idx_t k = n >> 1;                    /* floor(n/2) */
    seq_t bin = binomial(n, k);
    return motzkin[n] * bin;
}
