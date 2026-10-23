/*
 * A179635  Median of the digits in n (rounding down).
 * https://oeis.org/A179635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = median digit of n (rounding down)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset: a(1) .. */
    if (n == 0) return 0;  /* safety for non‑positive indices */

    /* Count digits of n */
    idx_t temp = n;
    idx_t digits = 0;
    while (temp > 0) {
        temp /= 10;
        digits++;
    }

    /* Position of the median digit (0‑based from the left) */
    idx_t k = (digits - 1) / 2;

    /* Compute 10^(digits - k - 1) */
    idx_t power = 1;
    idx_t shift = digits - k - 1;
    while (shift-- > 0) {
        power *= 10;
    }

    /* Extract the digit */
    idx_t digit = (n / power) % 10;
    return (seq_t)digit;
}
