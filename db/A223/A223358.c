/*
 * A223358  Rolling cube footprints: number of 3 X n 0..5 arrays starting with
 *          0 where 0..5 label faces of a cube and every array movement to a
 *          horizontal or antidiagonal neighbor moves across a corresponding
 *          cube edge.
 * https://oeis.org/A223358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 40*a(n-1) - 128*a(n-2) for n>4;
 * initial values: a(1)=36, a(2)=1024, a(3)=36864, a(4)=1327104.
 */
seq_t a(idx_t n)
{
    if (n == 1) return 36;
    if (n == 2) return 1024;
    if (n == 3) return 36864;
    if (n == 4) return 1327104;

    /* recurrence for n >= 5 */
    seq_t a_nm2 = 36864;    /* a(3) */
    seq_t a_nm1 = 1327104;  /* a(4) */
    seq_t cur = 0;

    for (idx_t i = 5; i <= n; ++i) {
        cur = 40 * a_nm1 - 128 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
