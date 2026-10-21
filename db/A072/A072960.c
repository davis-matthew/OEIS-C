/*
 * A072960  Numbers using only the curved digits 0, 3, 6, 8 and 9.
 * https://oeis.org/A072960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = convert (n-1) from base‑5 to decimal using the digit map
 *            0→0, 1→3, 2→6, 3→8, 4→9. The sequence is indexed starting at 1
 *            (offset 1)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case a(1) = 0 explicitly. */
    if (n == 0)
        return (seq_t)0;
    idx_t m = n - 1;        /* value to encode */

    /* Digit mapping for base‑5 digits. */
    int map[5] = {0, 3, 6, 8, 9};

    /* Gather base‑5 digits (least significant first). */
    int digits[64];          /* 64 digits is enough for all 64‑bit values */
    int len = 0;
    if (m == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (m > 0) {
            digits[len++] = (int)(m % 5);
            m /= 5;
        }
    }

    /* Build the decimal number using the mapping. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i)
        result = result * 10 + (seq_t)map[digits[i]];

    return result;
}
