/*
 * A021163  Decimal expansion of 1/159.
 * https://oeis.org/A021163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the n‑th decimal digit of 1/159. The result is obtained
 *            by performing long division. The period of the expansion is 52,
 *            so a(n) = digits[n mod 52]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* A static lookup table for the 52‑digit repeating block. */
    static seq_t digits[64];
    static idx_t period = 0;

    /* Initialise the table on the first call. */
    if (period == 0)
    {
        idx_t rem = 1;        /* current remainder */
        size_t i   = 0;       /* table index */

        do
        {
            seq_t d = (seq_t)((rem * 10) / 159); /* next decimal digit */
            digits[i++] = d;
            rem = (rem * 10) % 159;               /* new remainder */
        } while (rem != 1 && i < sizeof(digits) / sizeof(digits[0]));

        period = (idx_t)i; /* should be 52 for the period of 1/159 */
    }

    /* The sequence is periodic with the period found above. */
    size_t idx = (size_t)(n % period);   /* 0‑based index within the cycle */
    return digits[idx];
}
