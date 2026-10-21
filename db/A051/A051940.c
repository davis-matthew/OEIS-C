/*
 * A051940  Truncated triangular numbers: n*(n+1)/2 - 3*t*(t+1)/2 with t=4.
 * https://oeis.org/A051940
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)/2 - 30 , for n ≥ 8 (offset 8)
 * Range:     u8 n <= 23, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8) {
        /* Undefined for n < 8; return 0 or any placeholder */
        return 0;
    }
    const seq_t nn = (seq_t)n;
    return nn * (nn + 1) / 2 - 30;
}
