/*
 * A208114  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^4 if n is even * a(n) = n^2 (n+2)^2 if n is odd *
 *            (for n >= 1)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but guard for safety */

    seq_t sn = (seq_t)n;
    if ((n & 1) == 0) {            /* n even */
        seq_t m = sn + 1;          /* m = n+1 */
        return m * m * m * m;      /* (n+1)^4 */
    } else {                        /* n odd */
        seq_t n2 = sn * sn;         /* n^2 */
        seq_t m = sn + 2;           /* n+2 */
        seq_t m2 = m * m;           /* (n+2)^2 */
        return n2 * m2;             /* n^2 (n+2)^2 */
    }
}
