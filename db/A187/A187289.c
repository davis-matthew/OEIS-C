/*
 * A187289  Number of 4-step one or two space at a time rook's tours on an n X
 *          n board summed over all starting positions.
 * https://oeis.org/A187289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 380*n^2 - 1532*n + 1224 for n > 5 Recurrence: a(n) =
 *            3*a(n-1) - 3*a(n-2) + a(n-3) for n > 8 Initial values: a(1)=0,
 *            a(2)=8, a(3)=288, a(4)=1256, a(5)=3084, a(6)=5712, a(7)=9120,
 *            a(8)=13288
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[9] = {0, 0, 8, 288, 1256, 3084, 5712, 9120, 13288};
    if (n <= 8)
        return init[n];
    seq_t a_nm3 = init[6];   /* a(n-3)  with n=9 => a(6) */
    seq_t a_nm2 = init[7];   /* a(n-2)  with n=9 => a(7) */
    seq_t a_nm1 = init[8];   /* a(n-1)  with n=9 => a(8) */
    for (idx_t i = 9; i <= n; ++i)
    {
        seq_t val = 3 * a_nm1 - 3 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = val;
    }
    return a_nm1;
}
