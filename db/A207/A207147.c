/*
 * A207147  Number of (n+1) X 7 0..2 arrays with every 2 X 2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A207147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) for n > 13 with initial
 *            values: a(1) = 122, a(2) = 187, a(3) = 218, a(4) = 374, a(5) =
 *            434, a(6) = 754, a(7) = 866, a(8) = 1490, a(9) = 1730, a(10) =
 *            2962, a(11) = 3458, a(12) = 5906, a(13) = 6914
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 51, u64 n <= 115, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* behaviour below the offset is irrelevant */

    /* 1-based table with a(0)=0 unused */
    static const seq_t init[14] = {
        0,  /* a(0) not used */
        122, 187, 218, 374, 434,
        754, 866, 1490, 1730,
        2962, 3458, 5906, 6914
    };

    if (n <= 13) return init[n];

    /* a(k-3), a(k-2), a(k-1) for the current k */
    seq_t a_k3 = init[11];
    seq_t a_k2 = init[12];
    seq_t a_k1 = init[13];
    seq_t value;
    idx_t i;

    for (i = 14; i <= n; ++i) {
        value = a_k1 + ((seq_t)2 * a_k2) - ((seq_t)2 * a_k3);
        a_k3 = a_k2;
        a_k2 = a_k1;
        a_k1 = value;
    }
    return a_k1;
}
