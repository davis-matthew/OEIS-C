/*
 * A021460  Decimal expansion of 1/456.
 * https://oeis.org/A021460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A021460: Decimal expansion of 1/456.
   Formula: a(n) = floor(10^(n+1) / 456) modulo 10
   (the n-th digit after the decimal point, starting at n = 0).
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>   /* for uint64_t (idx_t) */

seq_t a(idx_t n)
{
    /* cached digits of the decimal expansion of 1/456 */
    static seq_t *digits = NULL;    /* array of currently stored digits */
    static size_t digits_len = 0;   /* number of digits stored so far */
    static seq_t remainder = (seq_t)1; /* remainder before generating next digit */

    /* target index (n) is nonnegative; for safety cast to size_t */
    size_t target = (size_t)n;

    /* expand cache if necessary */
    if (target >= digits_len) {
        size_t new_len = target + 1;

        if (digits == NULL) {
            /* first allocation */
            digits = (seq_t *)malloc(new_len * sizeof(seq_t));
            digits_len = 0;
        } else {
            /* grow existing array */
            seq_t *tmp = (seq_t *)realloc(digits, new_len * sizeof(seq_t));
            if (tmp == NULL) {
                /* allocation failure – abort (undefined behavior otherwise) */
                abort();
            }
            digits = tmp;
        }

        /* generate missing digits using long division
           starting from the current remainder */
        for (size_t i = digits_len; i <= target; ++i) {
            remainder = remainder * (seq_t)10;      /* shift left by decimal */
            seq_t digit = remainder / (seq_t)456;   /* next decimal digit */
            remainder = remainder % (seq_t)456;      /* new remainder */
            digits[i] = digit;
        }
        digits_len = new_len;
    }

    return digits[target];
}
