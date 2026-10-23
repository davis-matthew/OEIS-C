/*
 * A117640  Concatenation of first n numbers in base 4.
 * https://oeis.org/A117640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of the base‑4 representations of 1,2,…,n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t val = i;
        /* collect base‑4 digits of val in reverse order */
        unsigned char d[64];
        int len = 0;
        do {
            d[len++] = val & 3;   /* equiv. to val % 4 for unsigned */
            val >>= 2;           /* equiv. to val / 4 */
        } while (val);
        /* append digits to the big integer in correct order */
        for (int j = len - 1; j >= 0; --j)
            res = res * (seq_t)10 + (seq_t)d[j];
    }
    return res;
}
