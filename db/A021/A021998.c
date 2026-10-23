/*
 * A021998  Decimal expansion of 1/994.
 * https://oeis.org/A021998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n‑th digit after the decimal point in 1/994
 * Range:     u8 n <= 4, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for previously computed digits. */
static seq_t       *seq_cache = NULL;   /* array of digits, 0‑based */
static size_t        cache_len = 0;     /* number of digits stored */
static seq_t          remainder = 1;    /* current remainder for long division */

/* Return the n‑th digit of the decimal expansion of 1/994 */
seq_t a(idx_t n) {
    /* If the digit is already cached, return it. */
    if (n < cache_len)
        return seq_cache[n];

    /* Extend the cache up to position n. */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
    if (!tmp)                /* allocation failure – silently return 0 */
        return 0;

    seq_cache = tmp;
    seq_t denom = 994;

    /* Compute missing digits using long‑division. */
    for (size_t i = cache_len; i <= n; ++i) {
        seq_t ten   = remainder * ((seq_t)10);
        seq_t digit = ten / denom;
        seq_cache[i] = digit;
        remainder = ten % denom;
    }

    cache_len = new_len;
    return seq_cache[n];
}
