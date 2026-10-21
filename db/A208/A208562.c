/*
 * A208562  Number of n X 3 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) - 110*a(n-2) + 180*a(n-3) - 60*a(n-4) for n > 5
 *            Initial terms: a(1)=5, a(2)=68, a(3)=1059, a(4)=16608,
 *            a(5)=260856
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 5 first terms, can be used for n <= 5 and as seed for the recurrence */
    static const seq_t init[5] = {5, 68, 1059, 16608, 260856};

    if (n <= 5)
        return init[n - 1];

    /* Seed the recurrence with a(2)..a(5) */
    seq_t nm4 = init[1];  /* a(2) */
    seq_t nm3 = init[2];  /* a(3) */
    seq_t nm2 = init[3];  /* a(4) */
    seq_t nm1 = init[4];  /* a(5) */

    seq_t val;
    for (idx_t i = 6; i <= n; ++i) {
        val = ((seq_t)22 * nm1) - ((seq_t)110 * nm2)
              + ((seq_t)180 * nm3) - ((seq_t)60 * nm4);
        nm4 = nm3;
        nm3 = nm2;
        nm2 = nm1;
        nm1 = val;
    }
    return nm1;
}
