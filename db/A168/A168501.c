/*
 * A168501  Numbers without the decimal digits 2, 4 and 6.
 * https://oeis.org/A168501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = map_base7(n‑1) where the base‑7 digits 0..6 are mapped to
 *            {0,1,3,5,7,8,9}
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* should not occur; return 0 */
        return (seq_t)0;

    idx_t x = n - 1;               /* convert to base‑7 index */
    unsigned const map[7] = {0,1,3,5,7,8,9};
    unsigned digits[64];          /* enough for 64‑bit exponent */
    int len = 0;

    if (x == 0)                    /* n = 1 => number 0 */
        digits[len++] = 0;
    else
    {
        while (x > 0)
        {
            unsigned d = (unsigned)(x % 7);
            digits[len++] = map[d];
            x /= 7;
        }
    }

    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}
