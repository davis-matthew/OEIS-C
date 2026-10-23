/*
 * A179636  Median of the digits in n (rounding up).
 * https://oeis.org/A179636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the median (rounding up) digit of the decimal
 *            representation of n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count decimal digits of n */
    idx_t t = n;
    int d = 0;
    do {
        d++;
        t /= 10;
    } while (t);

    /* Position of the median digit (1‑based) */
    int pos = d / 2 + 1;  /* floor(d/2)+1 */

    /* Compute divisor = 10^(d - pos) */
    seq_t divisor = 1;
    for (int i = 0; i < d - pos; ++i) {
        divisor *= 10;
    }

    /* Extract the digit */
    seq_t nn = (seq_t)n;
    seq_t digit = (nn / divisor) % 10;
    return digit;
}
