/*
 * A059517  a(n) = A059515(3,n). Number of ways of placing n identifiable
 *          nonnegative intervals with a total of exactly three starting
 *          and/or finishing points.
 * https://oeis.org/A059517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 0, a(n) = 6^n - 3*3^n + 3 for n > 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pow6 = NULL;
static seq_t *pow3 = NULL;
static idx_t max_cached = 0;

/* make sure powers up to target are computed */
static void ensure_powers(idx_t target)
{
    if (target <= max_cached)
        return;

    if (pow6 == NULL) {
        /* first allocation */
        pow6 = (seq_t*)calloc(target + 1, sizeof(seq_t));
        pow3 = (seq_t*)calloc(target + 1, sizeof(seq_t));
        pow6[0] = (seq_t)1;
        pow3[0] = (seq_t)1;
        max_cached = 0;
    } else {
        /* extend arrays */
        pow6 = (seq_t*)realloc(pow6, (target + 1) * sizeof(seq_t));
        pow3 = (seq_t*)realloc(pow3, (target + 1) * sizeof(seq_t));
    }

    for (idx_t i = max_cached + 1; i <= target; ++i) {
        pow6[i] = pow6[i - 1] * (seq_t)6;
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }
    max_cached = target;
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)0;

    ensure_powers(n);

    seq_t p6 = pow6[n];
    seq_t p3 = pow3[n];
    seq_t result = p6 - ((seq_t)3 * p3) + ((seq_t)3);
    return result;
}
