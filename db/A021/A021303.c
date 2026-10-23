/*
 * A021303  Decimal expansion of 1/299.
 * https://oeis.org/A021303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the (n+1)-th digit after the decimal point in the
 *            expansion of 1/299 (a(0) is the first digit after the decimal)
 * Range:     u8 n <= 19, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* WARNING: The recurrence a(n) = a(n-1) – a(n-33) + a(n-34) is valid for n > 33,
   but the simplest direct computation is by long division of 1 by 299. */

/* The function keeps a static cache so that calls may be in any order
   and each digit is computed at most once.  All arithmetic is performed
   with integers only. */

#include <stdlib.h>   /* malloc, realloc */

/* The types seq_t and idx_t are defined by the build; we only use them. */
/* seq_t is an unsigned integer type (uint8_t … uint128 or big integer). */
/* idx_t is an unsigned integer type big enough for the indices. */

/* External declaration already present in the build:
   seq_t a(idx_t n);
   The following definition provides the implementation. */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *digits = NULL;     /* array of computed digits */
    static idx_t cached   = 0;       /* number of digits stored (size of array) */
    static seq_t rem      = 1;       /* current remainder after the last digit */

    /* Return cached value if possible */
    if (n < cached)
        return digits[n];

    /* Resize the array to hold up to index n */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(digits, new_size * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed: terminate program – the caller will not use the value. */
        abort();
    }
    digits = tmp;

    /* Compute new digits using long division: remainder*10 / 299. */
    for (idx_t i = cached; i <= n; ++i) {
        /* The remainder is always < 299, so temp < 2990 and fits in 64 bits. */
        uint64_t temp = (uint64_t)rem * 10ULL;
        seq_t digit = (seq_t)(temp / 299ULL);
        rem = (seq_t)(temp % 299ULL);
        digits[i] = digit;
    }

    cached = n + 1;
    return digits[n];
}
