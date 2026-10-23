/*
 * A107128  Divide the even digits of n by 2!.
 * https://oeis.org/A107128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number obtained from n by replacing every even decimal digit d by d/2
 *        (digit wise) and concatenating the resulting digits in the same
 *        positions.  For example: 12 → 11, 100 → 100, 14 → 12. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* single digit 0 becomes 0/2 = 0 */

    seq_t result = 0;
    seq_t factor = 1;
    idx_t x = n;

    while (x > 0) {
        int d = x % 10;             /* current least‑significant digit */
        int td = (d & 1) ? d : d >> 1; /* if even divide by 2, else keep */
        result += (seq_t)td * factor;
        factor *= 10;
        x /= 10;
    }
    return result;
}
