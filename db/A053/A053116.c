/*
 * A053116  a(n) = ((9*n+10)(!^9))/10, related to A045756 ((9*n+1)(!^9)
 *          9-factorials).
 * https://oeis.org/A053116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{k=1..n} (9*k + 10), a(0) = 1 (equivalently, a(0)=1 and
 *            a(n)=a(n-1)*(9*n+10) for n≥1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* base case a(0) = 1 */

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* factor = 9*k + 10; cast k to seq_t to avoid overflow in small types */
        seq_t factor = (seq_t)9 * (seq_t)k + (seq_t)10;
        result = result * factor;
    }
    return result;
}
