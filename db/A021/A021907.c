/*
 * A021907  Decimal expansion of 1/903.
 * https://oeis.org/A021907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the n-th digit (starting with n = 0) in the decimal
 *            expansion of 1/903 (the first digit after the decimal point)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *digits_cache = NULL;
static idx_t max_computed = 0;
static seq_t rem = 1;   /* remainder before computing the next decimal digit */

seq_t a(idx_t n)
{
    /* Ensure we have at least one digit cached */
    if (digits_cache == NULL) {
        digits_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!digits_cache) return 0;          /* allocation failed */
        /* compute the first decimal digit (n = 0) */
        digits_cache[0] = (rem * (seq_t)10) / (seq_t)903;
        rem = (rem * (seq_t)10) % (seq_t)903;
        max_computed = 0;
    }

    /* Expand the cache if a larger n was requested */
    if (n > max_computed) {
        seq_t *tmp = (seq_t *)realloc(digits_cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failed */
        digits_cache = tmp;
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            digits_cache[i] = (rem * (seq_t)10) / (seq_t)903;
            rem = (rem * (seq_t)10) % (seq_t)903;
        }
        max_computed = n;
    }

    return digits_cache[n];
}
