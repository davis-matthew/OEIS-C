/*
 * A101400  a(n) = a(n-1) + 2*a(n-2) + a(n-3) - a(n-4).
 * https://oeis.org/A101400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3) - a(n-4); Initial values: a(0)
 *            = 1, a(1) = 2, a(2) = 5, a(3) = 10
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 60, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)10;

    seq_t a0 = (seq_t)1;   // a(n-4)
    seq_t a1 = (seq_t)2;   // a(n-3)
    seq_t a2 = (seq_t)5;   // a(n-2)
    seq_t a3 = (seq_t)10;  // a(n-1)
    seq_t an = a3;         // placeholder for current term

    for (idx_t i = 4; i <= n; ++i)
    {
        an = a3 + ((seq_t)2 * a2) + a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
