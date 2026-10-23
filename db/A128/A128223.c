/*
 * A128223  a(n) = if n mod 2 = 0 then n*(n+1)/2 otherwise (n+1)^2/2-1.
 * https://oeis.org/A128223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n%2==0 then n*(n+1)/2 else (n+1)^2/2 - 1
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {
        return nn * (nn + 1) / 2;
    } else {
        seq_t n1 = nn + 1;
        return n1 * n1 / 2 - 1;
    }
}
