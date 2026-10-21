/*
 * A238908  Number of (n+1) X (3+1) 0..2 arrays with no element equal to all
 *          horizontal neighbors or equal to all vertical neighbors, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A238908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n−1) + 120*a(n−2) − 678*a(n−3) + 522*a(n−4) +
 *            432*a(n−5) − 81*a(n−6) (n ≥ 7) Initial terms (a(1) … a(6)): 54,
 *            486, 17496, 408726, 10789686, 274834944
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values for a(1) .. a(6).  They are computed exactly for all
       seq_t types that can hold them. */
    static const seq_t init[6] = {
        54ULL,            /* a(1) */
        486ULL,           /* a(2) */
        17496ULL,         /* a(3) */
        408726ULL,        /* a(4) */
        10789686ULL,      /* a(5) */
        274834944ULL      /* a(6) */
    };

    if (n == 0)        /* undefined for offset 1, return 0 for safety */
        return 0;

    if (n <= 6)
        return init[n - 1];

    /* Rolling window for the last six terms of the sequence. */
    seq_t a1 = init[0];
    seq_t a2 = init[1];
    seq_t a3 = init[2];
    seq_t a4 = init[3];
    seq_t a5 = init[4];
    seq_t a6 = init[5];
    seq_t next = 0;

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) = 22*a6 + 120*a5 - 678*a4 + 522*a3 + 432*a2 - 81*a1  */
        next = 22 * a6;
        next += 120 * a5;
        next -= 678 * a4;
        next += 522 * a3;
        next += 432 * a2;
        next -= 81 * a1;

        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return next;      /* a(n) */
}
