/*
 * A222249  In the number n, replace all (decimal) digits '6' with '7' and
 *          vice versa.
 * https://oeis.org/A222249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   replace decimal digits 6 with 7 and 7 with 6 in n's decimal
 *            representation
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res   = 0;   /* result to be returned */
    seq_t pow10 = 1;   /* current power of 10, fits in seq_t */
    idx_t tmp   = n;   /* temporary copy of the index */

    while (tmp > 0) {
        unsigned digit = tmp % 10;              /* current least‑significant digit */
        if (digit == 6)          digit = 7;
        else if (digit == 7)     digit = 6;
        res += (seq_t)digit * pow10;           /* add contribution of this digit */
        tmp /= 10;                               /* move to next more significant digit */
        if (tmp > 0)                    /* avoid overflow when the last digit has been processed */
            pow10 *= 10;
    }
    return res;   /* works also for n == 0 : loop is skipped, res stays 0 */
}
