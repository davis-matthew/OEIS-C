/*
 * A021869  Decimal expansion of 1/865.
 * https://oeis.org/A021869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/865) mod 10 (decimal digit of 1/865)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of already computed digits */
    static seq_t *digits = NULL;     /* array of digits, each 0..9 */
    static size_t len = 0;           /* number of digits currently stored */
    static size_t cap = 0;           /* allocated capacity of digits array */
    static uint64_t rem = 1;         /* current remainder for long‑division */

    /* if requested digit is already available, return it */
    if (n < len)
        return digits[n];

    /* grow the cache if necessary */
    if (cap < (size_t)(n + 1)) {
        size_t newcap = cap > 0 ? cap * 2 : 128;
        while (newcap < (size_t)(n + 1))
            newcap *= 2;

        seq_t *newptr = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (newptr == NULL)         /* allocation failed */
            return 0;

        if (digits) {
            memcpy(newptr, digits, len * sizeof(seq_t));
            free(digits);
        }
        digits = newptr;
        cap = newcap;
    }

    /* generate the missing digits using long division */
    for (idx_t i = len; i <= n; ++i) {
        rem *= 10ULL;                 /* shift numerator left by one decimal place */
        seq_t digit = rem / 865ULL;   /* next decimal digit of 1/865 */
        rem %= 865ULL;                /* update remainder for next step */
        digits[i] = digit;
    }

    len = (size_t)(n + 1);
    return digits[n];
}
