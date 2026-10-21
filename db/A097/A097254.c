/*
 * A097254  Numbers whose set of base 8 digits is {0,7}.
 * https://oeis.org/A097254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7 * A033045(n-1) A033045(k) is the number whose base‑8
 *            digits are 0 or 1 and equals the integer obtained by
 *            interpreting the binary expansion of k as base‑8 digits
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 according to the description; guard against 0 for safety */
    if (n == 0)
        return (seq_t)0;

    idx_t k = n - 1;          /* binary pattern to read */
    seq_t result = 0;         /* a(n) we are building */
    seq_t pow = 7;            /* current contribution 7 * 8^pos */

    while (k != 0) {
        if (k & 1)            /* if the bit at this position is 1 */
            result += pow;    /* add 7*8^pos to the result */

        pow *= 8;             /* move to the next higher digit */
        k >>= 1;              /* shift to the next bit */
    }

    return result;
}
