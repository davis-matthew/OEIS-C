/*
 * A055966  n + reversal of base 20 digits of n (written in base 10).
 * https://oeis.org/A055966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n + reverse_base20(n) (the base‑20 digit reversal of n,
 *            as a decimal integer)
 * Range:     u8 n <= 31, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* collect base‑20 digits of n (least significant first) */
    idx_t tmp = n;
    unsigned digits[64];
    int len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % 20;
        tmp /= 20;
    }

    /* build the reversed number in base 10 using 128‑bit arithmetic */
    unsigned __int128 rev = 0;
    for (int i = 0; i < len; ++i) {
        rev = rev * 20 + digits[i];
    }

    /* a(n) = n + reverse_base20(n) */
    unsigned __int128 sum = (unsigned __int128)n + rev;
    return (seq_t)sum;
}
