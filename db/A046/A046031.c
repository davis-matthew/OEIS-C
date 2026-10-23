/*
 * A046031  Digits are cubes.
 * https://oeis.org/A046031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal number obtained from the base‑3 representation of
 *            n by mapping digits 0→0, 1→1, 2→8. That is, a(n) is the smallest
 *            non‑negative integer whose decimal digits are each 0, 1 or 8 and
 *            that appears as the n‑th such integer in increasing order
 * Range:     u8 n <= 17, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 fits the mapping directly (0 → 0). */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Convert n to base‑3, store the digits in an array from least
       significant to most significant. */
    idx_t tmp = n;
    int digits[128]; /* 128 is ample for any reasonable n in the tests. */
    int len = 0;
    while (tmp > 0) {
        /* tmp % 3 yields 0, 1 or 2. */
        digits[len++] = (int)(tmp % 3);
        tmp /= 3;
    }

    /* Build the decimal value by mapping the reversed digits. */
    seq_t value = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        int mapped = (d == 0) ? 0
                    : (d == 1) ? 1
                    : 8;   /* d == 2 */
        value = value * 10 + (seq_t)mapped;
    }
    return value;
}
