/*
 * A121509  a(n) = 5*n^2/2 - 5*n + 13/4 - (-1)^n/4.
 * https://oeis.org/A121509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*(n-1)^2 + floor((n-1)^2/2)
 * Range:     u8 n <= 11, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based; avoid underflow for n == 0 */
    if (n == 0)
        return 0;            /* undefined for offset 1, safe fallback */

    seq_t s = (seq_t)(n - 1);    /* compute (n-1) */
    seq_t t = s * s;            /* (n-1)^2 */
    seq_t res = 1 + (t << 1) + (t >> 1); /* 1 + 2*t + floor(t/2) */
    return res;
}
