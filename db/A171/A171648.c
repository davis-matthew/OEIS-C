/*
 * A171648  a(1) = 1, a(n) = 2*a(n-1) if n is even; a(n) =
 *          a(n-1)*Fibonacci((n+1)/2)/Fibonacci((n-1)/2) if n is odd.
 * https://oeis.org/A171648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*a(n-2)+4*a(n-4) for n>4; a(1)=1, a(2)=2, a(3)=2, a(4)=4
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 39, u64 n <= 77, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* safety for n<1 */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;
    if (n == (idx_t)3) return (seq_t)2;
    if (n == (idx_t)4) return (seq_t)4;

    seq_t a0 = (seq_t)1; /* a(1)   */
    seq_t a1 = (seq_t)2; /* a(2)   */
    seq_t a2 = (seq_t)2; /* a(3)   */
    seq_t a3 = (seq_t)4; /* a(4)   */
    seq_t curr;

    for (idx_t i = (idx_t)5; i <= n; ++i) {
        curr = (seq_t)2 * a2;         /* 2*a(i-2) */
        curr += (seq_t)4 * a0;        /* +4*a(i-4) */
        a0 = a1;                      /* shift window */
        a1 = a2;
        a2 = a3;
        a3 = curr;
    }
    return curr;
}
