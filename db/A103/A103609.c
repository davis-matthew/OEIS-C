/*
 * A103609  Fibonacci numbers repeated (cf. A000045).
 * https://oeis.org/A103609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000045(floor(n/2)) where A000045 is the Fibonacci
 *            sequence with F(0)=0, F(1)=1
 * Range:     u8 n <= 27, u16 n <= 49, u32 n <= 95, u64 n <= 187,
 *            u128 n <= 373, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n >> 1;          /* floor(n/2) */
    if (m == 0) return (seq_t)0;

    seq_t f0 = 0;              /* F(0) */
    seq_t f1 = 1;              /* F(1) */
    idx_t i;

    for (i = 2; i <= m; ++i) {
        seq_t fn = f0 + f1;
        f0 = f1;
        f1 = fn;
    }
    return f1;
}
