/*
 * A108730  Triangle read by rows: row n gives the list of the number of zeros
 *          following each 1 in the binary representation of n.
 * https://oeis.org/A108730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nice, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 5120, u16 n <= 5120, u32 n <= 5120, u64 n <= 5120,
 *            u128 n <= 5120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108730: a(n) is the nth entry in the triangle read by rows;
   each row m lists the number of consecutive zeros following each 1
   in the binary representation of m.  The function generates rows
   incrementally until the requested index is reached. */
seq_t a(idx_t n)
{
    idx_t count = 0;      /* how many terms have been emitted so far */
    for (idx_t m = 1; ; ++m) {
        /* find the bit length of m */
        idx_t tmp = m;
        int len = 0;
        while (tmp) { len++; tmp >>= 1; }
        if (len == 0) len = 1;      /* should not happen for m>=1 */

        /* scan bits from most significant to least significant */
        for (int pos = len - 1; pos >= 0; --pos) {
            if ((m >> pos) & 1) {            /* a '1' bit found */
                /* count zeros after this 1 up to the next 1 or end */
                int zeros = 0;
                for (int k = pos - 1; k >= 0; --k) {
                    if ((m >> k) & 1) { break; }
                    zeros++;
                }
                count++;
                if (count == n) {
                    return (seq_t)zeros;
                }
            }
        }
    }
}
