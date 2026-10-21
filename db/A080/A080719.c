/*
 * A080719  Replace decimal digits with their binary values and convert back
 *          to decimal representation.
 * https://oeis.org/A080719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret as a decimal number the binary string obtained
 *            by concatenating the binary representations of the decimal
 *            digits of n
 * Range:     u8 n <= 187, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Number of bits needed to represent each decimal digit (remainders 0–9). */
    static const unsigned char bitlen_tbl[10] = {1,1,2,2,3,3,3,3,4,4};

    /* Store the decimal digits of n in reverse order. */
    unsigned digits[20];          /* 64‑bit n has at most 20 decimal digits. */
    int cnt = 0;
    idx_t tmp = n;
    do
    {
        digits[cnt++] = tmp % 10;
        tmp /= 10;
    } while (tmp > 0);

    /* Build the binary string and then interpret it as a decimal integer. */
    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i)
    {
        unsigned d   = digits[i];
        unsigned len = bitlen_tbl[d];
        res = (res << len) | (seq_t)d;
    }
    return res;
}
