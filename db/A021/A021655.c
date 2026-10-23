/*
 * A021655  Decimal expansion of 1/651.
 * https://oeis.org/A021655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/651) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    that is, a(n) is the n‑th digit after the decimal point of 1/651 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed digits and of the remainder
       after the last computed digit                                            */
    static seq_t  *digits   = NULL; /* cached digits 0 … len-1            */
    static idx_t  len       = 0;    /* number of digits cached            */
    static idx_t  cap       = 0;    /* current capacity of digits array    */
    static idx_t  rem       = 1;    /* remainder of previous step (<=650) */

    /* if we already have digit n, just return it */
    if (n < len)
        return digits[n];

    /* enlarge the array to hold at least n+1 digits */
    if ((idx_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 8)
            new_cap = 8;
        else if (new_cap < 2 * cap)
            new_cap = 2 * cap;
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        digits = tmp;
        cap = (idx_t)new_cap;
    }

    /* compute digits from the current length up to n */
    for (idx_t i = len; i <= n; ++i) {
        idx_t tmp = rem * 10ULL;      /* next remainder before division */
        seq_t digit = (seq_t)(tmp / 651ULL); /* next decimal digit */
        digits[i] = digit;
        rem = (idx_t)(tmp % 651ULL);      /* update remainder for next step */
    }
    len = n + 1;                       /* update length after computation */

    return digits[n];
}
