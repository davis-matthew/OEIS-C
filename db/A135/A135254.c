/*
 * A135254  Binomial transform of A131666.
 * https://oeis.org/A135254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(3)=4, a(n) = 6*a(n-1) - 12*a(n-2) +
 *            9*a(n-3) for n ≥ 4
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;

    /* Compute iteratively from a(0) to a(n) */
    seq_t a1 = (seq_t)0;   /* a(n-3)  */
    seq_t a2 = (seq_t)1;   /* a(n-2)  */
    seq_t a3 = (seq_t)4;   /* a(n-1)  */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = 6 * a3 - 12 * a2 + 9 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
