/*
 * A028046  Expansion of 1/((1-3x)(1-4x)(1-8x)(1-12x)).
 * https://oeis.org/A028046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-1) - 248*a(n-2) + 912*a(n-3) - 1152*a(n-4) Initial
 *            values: a(0)=1, a(1)=27, a(2)=481, a(3)=7203
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)481;
    if (n == 3) return (seq_t)7203;

    seq_t a0 = (seq_t)1;      /* a(n-4) */
    seq_t a1 = (seq_t)27;     /* a(n-3) */
    seq_t a2 = (seq_t)481;    /* a(n-2) */
    seq_t a3 = (seq_t)7203;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute next term using only addition, subtraction and
           multiplication by small constants; the intermediate
           arithmetic is performed in seq_t, which supports the
           operations on all integer types required by the build.   */
        seq_t x = (seq_t)27 * a3  +  (seq_t)912 * a1;
        seq_t y = (seq_t)248 * a2 + (seq_t)1152 * a0;
        seq_t next = x - y;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
