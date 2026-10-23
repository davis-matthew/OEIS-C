/*
 * A160380  a(0) = 0; for n >= 1, a(n) = number of 0's in base-4
 *          representation of n.
 * https://oeis.org/A160380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of 0's in the base‑4 representation of n, with
 *            a(0)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) is defined to be 0 */
        return 0;

    seq_t count = 0;
    while (n != 0) {
        /* least significant base‑4 digit is n & 3 */
        if ((n & 3U) == 0)
            count++;
        n >>= 2;   /* integer division by 4 */
    }

    return count;
}
