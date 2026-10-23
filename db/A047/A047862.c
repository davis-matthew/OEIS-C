/*
 * A047862  a(n) = T(5,n), array T given by A047858.
 * https://oeis.org/A047862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 4*a(n-3) (n >= 3); Initial values:
 *            a(0)=1, a(1)=7, a(2)=20
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)20;

    seq_t a0 = (seq_t)1;   // a(n-3)
    seq_t a1 = (seq_t)7;   // a(n-2)
    seq_t a2 = (seq_t)20;  // a(n-1)

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t ai = (seq_t)5 * a2 + (seq_t)4 * a0;   // 5*a(n-1) + 4*a(n-3)
        ai -= (seq_t)8 * a1;                        // subtract 8*a(n-2)
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
