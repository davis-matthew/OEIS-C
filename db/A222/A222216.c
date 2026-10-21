/*
 * A222216  In the number n, replace all (decimal) digits 0 with 7 and vice
 *          versa.
 * https://oeis.org/A222216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace each decimal digit 0 with 7 and each digit 7 with
 *            0 in n
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are provided by the build environment */
seq_t a(idx_t n)
{
    /* Special case for n = 0, whose decimal representation is "0".  The
       transformation turns it into "7". */
    if (n == 0)
        return (seq_t)7;          /* explicit cast for clarity */

    seq_t x     = (seq_t)n;  /* current working copy of n */
    seq_t result = 0;        /* transformed value being accumulated */
    seq_t mult   = 1;        /* positional multiplier (1, 10, 100, …) */

    while (x != 0)
    {
        seq_t digit = x % 10;               /* extract the least significant decimal digit */
        if (digit == 0)
            digit = 7;                       /* 0 → 7 */
        else if (digit == 7)
            digit = 0;                       /* 7 → 0 */

        result += digit * mult;             /* add the transformed digit to the result */
        mult   *= 10;                       /* move to the next decimal place */
        x      /= 10;                       /* remove the processed digit */
    }

    return result;                           /* final transformed number */
}
