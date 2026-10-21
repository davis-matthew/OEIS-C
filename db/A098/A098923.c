/*
 * A098923  33-gonal numbers: n(31n-29)/2.
 * https://oeis.org/A098923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(31*n-29)/2 (offset 0)
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  Use the parity of n to avoid overflow in the 64‑bit
     *  build, while remaining correct for arbitrary‑precision
     *  builds.  The expression n*(31*n-29) is always even,
     *  so we can divide by 2 in one of the factors. */
    seq_t N = n;                      /* convert index to seq_t */
    seq_t t = (seq_t)31 * N - (seq_t)29;   /* (31*n-29) */

    if ((N & 1) == 0) {               /* n even: divide N by 2 first */
        return (N / 2) * t;
    } else {                           /* n odd: divide t by 2 first */
        return N * (t / 2);
    }
}
