/*
 * A212677  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          w+y=|x-y|+|y-z|.
 * https://oeis.org/A212677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-5) -
 *            a(n-6) Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=7, a(4)=21,
 *            a(5)=46
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for the first six terms */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)21;
    if (n == 5) return (seq_t)46;

    /* Circular buffer holding the last seven values */
    seq_t buf[7];
    buf[0] = (seq_t)0;
    buf[1] = (seq_t)0;
    buf[2] = (seq_t)1;
    buf[3] = (seq_t)7;
    buf[4] = (seq_t)21;
    buf[5] = (seq_t)46;

    /* Compute terms up to n */
    for (idx_t i = 6; i <= n; ++i) {
        idx_t m = i % 7;
        seq_t val =
            3 * buf[(i - 1) % 7]
          - 3 * buf[(i - 2) % 7]
          + 2 * buf[(i - 3) % 7]
          - 3 * buf[(i - 4) % 7]
          + 3 * buf[(i - 5) % 7]
          - buf[(i - 6) % 7];
        buf[m] = val;
    }

    return buf[n % 7];
}
