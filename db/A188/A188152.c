/*
 * A188152  Number of 7-step self-avoiding walks on an n X n square summed
 *          over all starting positions.
 * https://oeis.org/A188152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n>8 with the first eight
 *            terms: a(1)=0, a(2)=0, a(3)=112, a(4)=1248, a(5)=4152,
 *            a(6)=8752, a(7)=14932, a(8)=22672
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base_vals[8] = {
    0,     /* a(1) */
    0,     /* a(2) */
    112,   /* a(3) */
    1248,  /* a(4) */
    4152,  /* a(5) */
    8752,  /* a(6) */
    14932, /* a(7) */
    22672  /* a(8) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, safety      */
    if (n <= 8) return base_vals[n - 1];

    /* Initialize the three previous values for the recurrence:
       a(n-3) = a(6), a(n-2) = a(7), a(n-1) = a(8) when n = 9. */
    seq_t a_nm3 = base_vals[5]; /* a(6) */
    seq_t a_nm2 = base_vals[6]; /* a(7) */
    seq_t a_nm1 = base_vals[7]; /* a(8) */
    seq_t cur = a_nm1;

    for (idx_t i = 9; i <= n; ++i) {
        cur = 3 * a_nm1 - 3 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}
