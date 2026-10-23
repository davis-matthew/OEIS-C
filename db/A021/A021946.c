/*
 * A021946  Decimal expansion of 1/942.
 * https://oeis.org/A021946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth decimal digit of 1/942 (decimal expansion),
 *            0-indexed
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *digits = NULL;
    static size_t   cap     = 0;
    static idx_t    len     = 0;      /* number of digits already computed */
    static uint64_t rem     = 1;      /* remainder before generating next digit */

    if (!digits) {
        cap = 64;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* compute digits up to index n */
    while (len <= n) {
        if (len == cap) {
            cap *= 2;
            seq_t *tmp = (seq_t *)realloc(digits, cap * sizeof(seq_t));
            digits = tmp;
        }

        rem *= 10;                                   /* bring down next digit */
        seq_t d = (seq_t)(rem / 942);                /* integer part */
        rem   %= 942;                                 /* new remainder */

        digits[len++] = d;
    }

    return digits[n];
}
