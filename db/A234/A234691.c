/*
 * A234691  Decimal value of the bitmap of active segments in 7-segment
 *          display of the number n, variant 1: bits 0-6 refer to segments
 *          from top to bottom, left to right.
 * https://oeis.org/A234691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n mod 10) + a(floor(n/10)) << 7
 * Range:     u8 n <= 9, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t DIG[10] = {119,36,93,109,46,107,123,39,127,111};

seq_t a(idx_t n)
{
    if (n == 0) return DIG[0];
    seq_t res = 0;
    /* collect 7‑bit patterns of the decimal digits */
    seq_t stack[20];
    int k = 0;
    while (n != 0)
    {
        stack[k++] = DIG[n % 10];
        n /= 10;
    }
    /* combine them from most significant to least */
    for (int i = k - 1; i >= 0; --i)
        res = (res << 7) | stack[i];
    return res;
}
