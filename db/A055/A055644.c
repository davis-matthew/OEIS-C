/*
 * A055644  Integers in base 20 with each base 20 digit represented by 2
 *          decimal digits.
 * https://oeis.org/A055644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal number obtained from the base‑20 representation
 *            of n, where each base‑20 digit is written as a two‑digit decimal
 *            field (with leading zeros)
 * Range:     u8 n <= 59, u16 n <= 2799, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    if (n == 0) return 0;          /* special case: n = 0 */

    /* buffer for base‑20 digits (80 digits are more than enough for any idx_t) */
    int digits[80];
    int len = 0;

    /* extract base‑20 digits, least‑significant first */
    idx_t tmp = n;
    while (tmp) {
        digits[len++] = (int)(tmp % 20);
        tmp /= 20;
    }

    /* build the decimal value, processing digits from most‑to‑least */
    for (int i = len - 1; i >= 0; --i) {
        result = result * 100 + digits[i];
    }

    return result;
}
