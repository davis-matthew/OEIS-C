/*
 * A061776  Start with a single triangle; at n-th generation add a triangle at
 *          each vertex, allowing triangles to overlap; sequence gives number
 *          of triangles in n-th generation.
 * https://oeis.org/A061776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=6, a(3)=12, a(n)=a(n-1)+2*a(n-2)-2*a(n-3)
 *            for n>3
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 58, u64 n <= 122,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)12;

    /* Maintain the last three computed values:
       a1 = a(n-3), a2 = a(n-2), a3 = a(n-1)  */
    seq_t a1 = (seq_t)3;          /* a(1) */
    seq_t a2 = (seq_t)6;          /* a(2) */
    seq_t a3 = (seq_t)12;         /* a(3) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = a3 + (seq_t)2 * a2 - (seq_t)2 * a1;
        /* Shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
