/*
 * A017899  Expansion of 1/(1 -x^5 -x^6 -x^7 - ...).
 * https://oeis.org/A017899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-5); a(0)=1; a(1)=a(2)=a(3)=a(4)=0
 * Range:     u8 n <= 27, u16 n <= 46, u32 n <= 86, u64 n <= 165,
 *            u128 n <= 322, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t buf[5] = {0};
    buf[0] = 1;
    if (n < 5) return buf[n];
    for (idx_t i = 5; i <= n; ++i)
        buf[i % 5] = buf[(i - 1) % 5] + buf[(i - 5) % 5];
    return buf[n % 5];
}
