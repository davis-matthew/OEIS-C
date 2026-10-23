/*
 * A039966  a(0) = 1; thereafter a(3n+2) = 0, a(3n) = a(3n+1) = a(n).
 * https://oeis.org/A039966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; thereafter a(3n+2) = 0, a(3n) = a(3n+1) = a(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    for (;;)
    {
        if (n == 0)
            return (seq_t)1;
        if (n % 3 == 2)
            return (seq_t)0;
        n /= 3;
    }
}
