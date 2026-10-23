/*
 * A021690  Decimal expansion of 1/686.
 * https://oeis.org/A021690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n-th digit after the decimal point of 1/686 (decimal
 *            expansion of 1/686)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *digits = NULL;    /* array of computed digits */
    static size_t  cap    = 0;      /* capacity of digits array          */
    static idx_t  last   = (idx_t)-1;/* index of last computed digit      */
    static idx_t  rem    = 1;       /* current remainder (0..685)         */

    /* return already computed digit */
    if (last != (idx_t)-1 && n <= last)
        return digits[n];

    /* ensure buffer can hold digits[0 .. n] */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap : 16;      /* start with 16 entries   */
        while ((size_t)n >= new_cap)
            new_cap <<= 1;                   /* grow exponentially      */
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        digits = tmp;
        cap    = new_cap;
    }

    /* compute missing digits from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        rem = rem * 10;                          /* bring next decimal digit */
        seq_t d = (seq_t)(rem / 686);            /* next digit              */
        rem    = rem % 686;                      /* update remainder        */
        digits[i] = d;                           /* store digit             */
    }

    last = n;                                    /* update last computed    */
    return digits[n];
}
