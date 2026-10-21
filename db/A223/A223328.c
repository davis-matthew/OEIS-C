/*
 * A223328  Rolling cube footprints: number of n X 5 0..7 arrays starting with
 *          0 where 0..7 label vertices of a cube and every array movement to
 *          a horizontal or antidiagonal neighbor moves along a corresponding
 *          cube edge.
 * https://oeis.org/A223328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 117*a(n-1) - 294*a(n-2) with initial values a(1)=81,
 *            a(2)=9261
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* undefined offset */
    if (n == 1) return (seq_t)81;
    if (n == 2) return (seq_t)9261;

    seq_t a_nm1 = (seq_t)81;   /* a(n-2) for i=3 */
    seq_t a_nm2 = (seq_t)9261; /* a(n-1) for i=3 */
    seq_t a_n;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = a_nm2 * (seq_t)117 - a_nm1 * (seq_t)294;
        a_nm1 = a_nm2;
        a_nm2 = a_n;
    }
    return a_nm2;
}
