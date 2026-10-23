/*
 * A187293  Number of 8-step one or two space at a time rook's tours on an n X
 *          n board summed over all starting positions.
 * https://oeis.org/A187293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 708492*n^2 - 5580668*n + 10074432 for n>13; otherwise use
 *            the provided values for n = 1 ... 13
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t table[13] = {
        0,          /* a(1) */
        0,          /* a(2) */
        2664,       /* a(3) */
        134944,     /* a(4) */
        866888,     /* a(5) */
        2732016,    /* a(6) */
        6049424,    /* a(7) */
        10906120,   /* a(8) */
        17282088,   /* a(9) */
        25129488,   /* a(10) */
        34417440,   /* a(11) */
        45129696,   /* a(12) */
        57260948    /* a(13) */
    };

    if (n == 0)
        return (seq_t)0;                 /* undefined for 0, safe default */

    if (n <= 13)
        return table[n - 1];

    seq_t N = n;
    return (seq_t)708492 * N * N
         - (seq_t)5580668 * N
         + (seq_t)10074432;
}
