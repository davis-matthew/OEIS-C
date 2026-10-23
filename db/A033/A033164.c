/*
 * A033164  Begins with (4, 5); avoids 3-term arithmetic progressions.
 * https://oeis.org/A033164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 + value obtained by interpreting the binary
 *            representation of (n-1) as a base‑3 number (t(0)=0, t(1)=1,
 *            t(2)=3, t(3)=4,...)
 * Range:     u8 n <= 36, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n - 1;        /* term index in the 0‑based sequence */
    seq_t val = 0;          /* value of t(k) */

    if (k > 0) {
        /* determine number of bits in k */
        idx_t msb = 0;
        idx_t t = k;
        while (t > 0) {
            msb++;
            t >>= 1;
        }

        /* process bits from most significant to least significant */
        for (idx_t i = msb; i > 0; --i) {
            val = val * 3 + ((k >> (i - 1)) & 1);
        }
    }

    return val + 4;          /* shift by 4 to match the beginning (4,5) */
}
