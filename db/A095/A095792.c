/*
 * A095792  a(n) = Z(n) - L(n), where Z=A072649 and L=A095791 are lengths of
 *          Zeckendorf and lazy Fibonacci representations in binary notation.
 * https://oeis.org/A095792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if (n+1) is a Fibonacci number, 1 otherwise
 * Range:     u8 n <= 4183, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + (seq_t)1;   /* m = n+1 */
    seq_t f1 = (seq_t)1;             /* F(1)  */
    seq_t f2 = (seq_t)1;             /* F(2)  */

    /* Handle the first two Fibonacci numbers */
    if (m == f1 || m == f2)
        return (seq_t)0;

    /* Generate Fibonacci numbers until we reach or surpass m */
    while (f2 < m) {
        seq_t f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }

    /* If the last Fibonacci number equals m, return 0; otherwise 1 */
    return (f2 == m) ? (seq_t)0 : (seq_t)1;
}
