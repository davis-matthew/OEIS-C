/*
 * A021126  Decimal expansion of 1/122.
 * https://oeis.org/A021126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth decimal digit (0‑based) of the expansion of
 *            1/122
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  static state to cache previously computed digits */
    static seq_t *digits = NULL;      /* array of computed digits */
    static idx_t  cap    = 0;         /* current capacity of digits array */
    static idx_t  next   = 0;         /* next index that has not yet been computed */
    static idx_t  rem    = 1;         /* remainder before computing digit 'next'  */

    /*  Ensure we have computed up to index n  */
    while (next <= n)
    {
        /*  Allocate or grow the digits array as needed  */
        if (next >= cap)
        {
            idx_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(digits, (size_t)new_cap * sizeof(seq_t));
            digits = tmp;                /* assume realloc succeeds in test env */
            cap    = new_cap;
        }

        /*  Long‑division step: multiply current remainder by 10  */
        unsigned long long temp   = (unsigned long long)rem * 10ULL;
        seq_t             digit   = (seq_t)(temp / 122ULL);   /* 0 ≤ digit ≤ 9  */
        rem = (idx_t)(temp % 122ULL);                         /* remainder < 122 */

        /*  Store the digit and advance to the next one  */
        digits[next++] = digit;
    }

    /*  Return the requested digit  */
    return digits[n];
}
