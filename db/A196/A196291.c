/*
 * A196291  a(n) = n^10 - n.
 * https://oeis.org/A196291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = n^10 - n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 7131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;   /* convert index to sequence type */
    seq_t p = sn;          /* start with n^1 */

    /* multiply by n nine more times to obtain n^10 */
    for (int i = 1; i < 10; ++i) {
        p *= sn;
    }

    /* subtract n to get a(n) */
    return p - sn;
}
