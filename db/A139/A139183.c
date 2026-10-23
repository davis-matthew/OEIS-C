/*
 * A139183  a(n) = (n! - 8)/8.
 * https://oeis.org/A139183
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! - 8)/8 – for n >= 4
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;                         /* running product of adjusted factors */
    unsigned int twos_needed = 3;          /* remove a factor of 8 from n! */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t val = (seq_t)i;              /* current factor */
        /* strip as many 2's as possible from this factor until we have removed 8 */
        while (((val & (seq_t)1) == 0) && twos_needed > 0) {
            val >>= 1;                    /* divide by 2 */
            --twos_needed;
        }
        res *= val;                        /* multiply the remaining part */
    }

    /* At this point res = n! / 8.  For n >= 4 this is an integer. */
    return res - (seq_t)1;                 /* a(n) = n!/8 - 1 = (n! - 8)/8 */
}
