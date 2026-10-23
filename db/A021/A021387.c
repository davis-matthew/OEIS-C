/*
 * A021387  Decimal expansion of 1/383.
 * https://oeis.org/A021387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th decimal digit of 1/383 after the decimal point
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Return the n-th decimal digit (0‑based) of the reciprocal 1/383.  */
seq_t a(idx_t n)
{
    /* static state preserved across calls */
    static seq_t *digits = NULL;   /* array holding already computed digits   */
    static size_t   len    = 0;    /* number of digits stored                */
    static uint64_t rem    = 1;    /* current remainder of the division      */

    /* guard against n too large for the machine size_t type */
    if (n >= (idx_t)SIZE_MAX) return (seq_t)0;

    size_t target = (size_t)n;

    if (target >= len)
    {
        /* allocate a new, larger array and copy previous digits   */
        size_t new_len = target + 1;
        seq_t *new_digits = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_digits) return (seq_t)0;   /* allocation failure – return 0 */

        for (size_t i = 0; i < len; ++i) new_digits[i] = digits[i];
        if (digits) free(digits);

        /* generate decimal digits from the current position to target */
        for (size_t i = len; i <= target; ++i)
        {
            rem *= 10ULL;                     /* shift for next decimal place */
            seq_t d = (seq_t)(rem / 383ULL);  /* next digit 0‑9               */
            new_digits[i] = d;
            rem = rem % 383ULL;
        }

        digits = new_digits;
        len    = new_len;
    }

    return digits[target];
}
