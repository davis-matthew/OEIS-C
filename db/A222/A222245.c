/*
 * A222245  In the number n, replace all (decimal) digits '5' with '6' and
 *          vice versa.
 * https://oeis.org/A222245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace each decimal digit 5 with 6 and each 6 with 5 in
 *            n
 * Range:     u8 n <= 249, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e. apply the digit swap 5↔6 to the decimal representation of n) */
seq_t a(idx_t n)
{
    seq_t result = 0;    /* the value to return */
    seq_t mult   = 1;    /* current power of 10 multiplier */
    idx_t  x     = n;    /* working copy of the input index */

    while (x > 0) {
        unsigned digit = x % 10;            /* current decimal digit */
        unsigned mapped = digit;            /* digit after swapping 5↔6 */

        if (digit == 5)          mapped = 6;
        else if (digit == 6)     mapped = 5;

        result += (seq_t)mapped * mult;     /* add the mapped digit */
        mult   *= (seq_t)10;                /* advance the multiplier */
        x     /= 10;                        /* move to the next higher digit */
    }
    return result;   /* for n == 0 the loop is skipped, result remains 0 */
}
