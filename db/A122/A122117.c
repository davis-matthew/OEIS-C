/*
 * A122117  a(n) = 3*a(n-1) + 4*a(n-2), with a(0)=1, a(1)=2.
 * https://oeis.org/A122117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return 1;
    if (n == 1u) return 2;

    seq_t a0 = 1;
    seq_t a1 = 2;

    for (idx_t i = 2u; i <= n; ++i)
    {
        seq_t next = a1 * 3 + a0 * 4;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
