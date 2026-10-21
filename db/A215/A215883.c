/*
 * A215883  When written in base 4, n ends in a(n) consecutive nonzero digits.
 * https://oeis.org/A215883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of trailing non‑zero digits in the base‑4
 *            representation of n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t tmp   = n;
    while (tmp != 0)
    {
        if ((tmp & 3) == 0)    /* least significant base‑4 digit is 0 */
            break;
        ++count;
        tmp >>= 2;             /* divide by 4 */
    }
    return count;
}
