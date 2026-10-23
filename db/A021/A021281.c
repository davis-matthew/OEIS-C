/*
 * A021281  Decimal expansion of 1/277.
 * https://oeis.org/A021281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/277
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by long division; the period equals the multiplicative
 * order of 10 modulo 277 (≤ 276).  The function caches the digits
 * once and returns the nth digit (0‑based). */
seq_t a(idx_t n)
{
    /* static state: period and digits */
    static int initialized = 0;
    static seq_t digits[276];
    static idx_t period = 0;

    if (!initialized) {
        /* compute the repeating block of digits */
        idx_t rem = 1;          /* remainder of 1/277 after decimal point */
        idx_t i = 0;
        do {
            rem *= 10;
            seq_t d = rem / 277;     /* current digit */
            rem = rem % 277;         /* new remainder */
            digits[i++] = d;         /* store digit */
        } while (rem != 1);          /* stop after completing the cycle */
        period = i;
        initialized = 1;
    }

    /* return the digit corresponding to position n */
    return digits[n % period];
}
