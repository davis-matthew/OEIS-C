/*
 * A229975  The base 8 expansion of the number of trailing zeros of the base 8
 *          expansion of (8^n)!.
 * https://oeis.org/A229975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the decimal number obtained by writing the base‑8 digits
 *            of floor((8^n-1)/3) in decimal. This is equivalent to dividing a
 *            string of '7' digits (length n) by 3 in base‑8 and concatenating
 *            the quotient digits as decimal digits
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* For indices below the offset, the value is irrelevant. */
        return 0;
    }
    seq_t result = 0;
    int carry = 0;          /* 0, 1 or 2 */
    for (idx_t i = 0; i < n; ++i) {
        int current = carry * 8 + 7;   /* current < 23 */
        int q = current / 3;           /* quotient digit (0…7) */
        carry = current % 3;           /* remainder (0…2) */
        result = result * 10 + q;      /* append decimal digit */
    }
    return result;
}
