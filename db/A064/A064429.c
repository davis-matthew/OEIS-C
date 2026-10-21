/*
 * A064429  a(n) = floor(n / 3) * 3 + sign(n mod 3) * (3 - n mod 3).
 * https://oeis.org/A064429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (n mod 3 == 1 ? 1 : n mod 3 == 2 ? -1 : 0)
 * Range:     u8 n <= 255, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is an idx_t (unsigned integer), n%3 fits in a very small integer */
    idx_t r = n % 3;
    seq_t res = (seq_t)n;

    if (r == 1) {
        return res + 1;
    } else if (r == 2) {
        return res - 1;  /* n >= 2 here, so no underflow */
    } else {
        return res;      /* r == 0 */
    }
}
