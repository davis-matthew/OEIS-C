/*
 * A222215  In the number n, replace all (decimal) digits '0' with '6' and
 *          vice versa.
 * https://oeis.org/A222215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace every decimal digit 0 by 6 and every digit 6 by 0
 *            in n
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;  /* a(0) = 6 */

    seq_t result = 0;
    seq_t factor = 1;

    while (n > 0) {
        unsigned int d   = (unsigned int)(n % 10);        /* current decimal digit */
        unsigned int mapped = (d == 0) ? 6 : (d == 6) ? 0 : d;
        result += (seq_t)mapped * factor;                /* add mapped digit */
        factor *= (seq_t)10;                              /* next decimal place */
        n /= 10;                                          /* progress to next digit */
    }

    return result;
}
