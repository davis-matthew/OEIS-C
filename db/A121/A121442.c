/*
 * A121442  Expansion of (1-x^2)/(1-x-9*x^2+x^3).
 * https://oeis.org/A121442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=1, a(2)=9; a(n+1)=a(n)+9*a(n-1)-a(n-2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)9;

    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)1;          /* a(1) */
    seq_t a2 = (seq_t)9;          /* a(2) */
    const seq_t nine = (seq_t)9;  /* constant 9 */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = a2 + nine * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
