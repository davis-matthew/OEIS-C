/*
 * A021640  Decimal expansion of 1/636.
 * https://oeis.org/A021640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^{n+1}/636) % 10
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the decimal digits of 1/636. */

#include <stdlib.h>

/* Static cache for previously computed digits. */
static seq_t *digits = NULL;        /* array of digits 0‑9 */
static size_t digits_len = 0;        /* number of digits stored */
static seq_t remainder = 1;          /* 10^digits_len mod 636 */

/* Return the n-th digit after the decimal point of 1/636. */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    if (target < digits_len)
        return digits[target];

    /* Grow the cache to at least target+1 elements. */
    size_t new_cap = digits_len ? digits_len : 4;
    while (new_cap <= target)
        new_cap <<= 1;

    if (!digits) {
        digits = (seq_t*)malloc(new_cap * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t*)realloc(digits, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failure – return 0 as fallback */
            return 0;
        digits = tmp;
    }

    /* Compute missing digits sequentially. */
    for (idx_t i = digits_len; i <= target; ++i) {
        seq_t digit = (remainder * 10) / 636;
        digits[(size_t)i] = digit;
        remainder = (remainder * 10) % 636;
    }

    digits_len = target + 1;
    return digits[(size_t)target];
}
