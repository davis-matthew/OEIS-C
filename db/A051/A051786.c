/*
 * A051786  Propp's cubic recurrence: a(0)=a(1)=a(2)=a(3)=1; for n>3,
 *          a(n)=(1+a(n-1)*a(n-2)*a(n-3))/a(n-4).
 * https://oeis.org/A051786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0)=a(1)=a(2)=a(3)=1; for n>3, a(n) = (1 +
 *            a(n-1)*a(n-2)*a(n-3))/a(n-4)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 1;

    seq_t a0 = 1, a1 = 1, a2 = 1, a3 = 1;

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t prod = a3 * a2;
        prod = prod * a1;
        seq_t next = (1 + prod) / a0;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
