/*
 * A080877  a(n)*a(n+3) - a(n+1)*a(n+2) = 2^n, given a(0)=1, a(1)=1, a(2)=2.
 * https://oeis.org/A080877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = 6*a(n+2) - 4*a(n), a(0)=1, a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base[4] = {(seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3};

    if (n < 4)
        return base[n];

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (seq_t)6 * base[(i - 2) % 4] - (seq_t)4 * base[(i - 4) % 4];
        base[i % 4] = next;
    }

    return base[n % 4];
}
