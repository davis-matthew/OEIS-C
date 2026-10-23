/*
 * A122641  {2n+1}_n.
 * https://oeis.org/A122641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the decimal expansion of (2 n + 1) as a base‑n
 *            number
 * Range:     u8 n <= 36, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2*n + 1 in seq_t */
    seq_t m = (seq_t)n * 2 + 1;

    /* Extract decimal digits of m (least significant first) */
    int digits[32];
    int len = 0;
    while (m > 0) {
        digits[len++] = (int)(m % 10);
        m = m / 10;
    }

    /* Reinterpret these digits in base n */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * n + (seq_t)digits[i];
    }

    return result;
}
