/*
 * A034793  a(1)=1; thereafter a(n+1) is the least k > a(n) such that k is a
 *          square mod a(i) for all i<= n.
 * https://oeis.org/A034793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=9, a(6)=13, * a(n) =
 *            (n-3)^2 for n >= 7 * This follows the proven pattern for all n
 *            >= 7 (conjectured to hold for all n * unconditionally up to at
 *            least 10^12.5)
 * Range:     u8 n <= 18, u16 n <= 258, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the known first six terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)4;
    if (n == 5) return (seq_t)9;
    if (n == 6) return (seq_t)13;

    /* For n >= 7, a(n) = (n-3)^2 */
    /* Since n >= 7, (n-3) >= 4, so no underflow in idx_t subtraction. */
    seq_t m = (seq_t)(n - (idx_t)3);
    return m * m;
}
