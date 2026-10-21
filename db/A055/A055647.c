/*
 * A055647  Integers in base 14 with each base 14 digit represented by 2
 *          decimal digits.
 * https://oeis.org/A055647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Concatenate the base‑14 digits of n, each digit written
 *            as two decimal digits (padded with a leading 0 for <10).
 *            Equivalent to: for the base‑14 digits d_k … d_0, a(n) =
 *            (((d_k)*100 + d_{k-1}) … )*100 + d_0
 * Range:     u8 n <= 41, u16 n <= 1371, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* a(0) = 0 */

    /* The most digits needed for any 64‑bit n in base 14 are less than 20.
       A fixed small array suffices for all expected input sizes. */
    idx_t digits[20];
    int len = 0;

    /* Extract base‑14 digits (least significant first). */
    while (n != 0) {
        digits[len++] = n % 14;
        n /= 14;
    }

    /* Build the resulting decimal number from the most significant digit. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * 100 + (seq_t)digits[i];
    }
    return result;
}
