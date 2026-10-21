/*
 * A219293  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or antidiagonal neighbors
 *          in a random, but sorted with lexicographically nondecreasing rows
 *          and columns, 0..2 n X 2 array.
 * https://oeis.org/A219293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n > 7 Initial values: a(1)=3, a(2)=7, a(3)=17, a(4)=35, a(5)=62,
 *            a(6)=103, a(7)=165
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first seven terms explicitly */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)17;
    if (n == 4) return (seq_t)35;
    if (n == 5) return (seq_t)62;
    if (n == 6) return (seq_t)103;
    if (n == 7) return (seq_t)165;

    /* v1..v5 hold a(n-5)..a(n-1) for the current n */
    seq_t v1 = (seq_t)17;   /* a(3)   */
    seq_t v2 = (seq_t)35;   /* a(4)   */
    seq_t v3 = (seq_t)62;   /* a(5)   */
    seq_t v4 = (seq_t)103;  /* a(6)   */
    seq_t v5 = (seq_t)165;  /* a(7)   */
    seq_t cur;

    for (idx_t i = 8; i <= n; ++i) {
        /* a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) */
        cur = 5 * v5 - 10 * v4 + 10 * v3 - 5 * v2 + v1;

        /* Shift the window for the next iteration */
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = v5;
        v5 = cur;
    }

    return v5;
}
