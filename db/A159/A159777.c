/*
 * A159777  Positive numbers y such that y^2 is of the form x^2+(x+167)^2 with
 *          integer x.
 * https://oeis.org/A159777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) (for n>6); a(1)=145, a(2)=167,
 *            a(3)=197, a(4)=673, a(5)=835, a(6)=1037
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 30, u64 n <= 69, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n == 1) return (seq_t)145;
    if (n == 2) return (seq_t)167;
    if (n == 3) return (seq_t)197;
    if (n == 4) return (seq_t)673;
    if (n == 5) return (seq_t)835;
    if (n == 6) return (seq_t)1037;

    /* v[0] = a(i-6), v[1] = a(i-5), v[2] = a(i-4),
       v[3] = a(i-3), v[4] = a(i-2), v[5] = a(i-1)  */
    seq_t v[6] = { (seq_t)145, (seq_t)167, (seq_t)197,
                   (seq_t)673, (seq_t)835, (seq_t)1037 };

    /* Iterate from i=7 up to the requested index n */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = (seq_t)6 * v[3] - v[0];
        /* Shift the window: discard a(i-6) and make room for a(i) */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }

    /* After the loop, v[5] holds a(n)  */
    return v[5];
}
