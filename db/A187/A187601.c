/*
 * A187601  n/2 times period 6 sequence [1, 2, 3, 4, 3, 2, ...].
 * https://oeis.org/A187601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = n*(5-2*(-1)^floor((n+1)/3)-(-1)^n)/4
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* (-1)^floor((n+1)/3) : +1 if floor((n+1)/3) even, -1 otherwise */
    idx_t p = (n + 1) / 3;
    int s1 = (p & 1) == 0 ? 1 : -1;

    /* (-1)^n */
    int s2 = (n & 1) == 0 ? 1 : -1;

    /* multiply factor: 5 - 2*s1 - s2  (always 2,4,6 or 8) */
    int factor = 5 - 2 * s1 - s2;

    seq_t result = nn * (seq_t)factor / 4;
    return result;
}
