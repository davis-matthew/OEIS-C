/*
 * A221829  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal, vertical or antidiagonal neighbor, without
 *          2-loops or left turns.
 * https://oeis.org/A221829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2) for n>4; a(1)=0, a(2)=3, a(3)=16,
 *            a(4)=73
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)16;
    if (n == 4) return (seq_t)73;

    seq_t a_nm2 = (seq_t)16; /* a(n-2) when n starts at 5 */
    seq_t a_nm1 = (seq_t)73; /* a(n-1) when n starts at 5 */
    seq_t a_n;

    for (idx_t i = 5; i <= n; ++i) {
        a_n    = (seq_t)5 * a_nm1 - (seq_t)2 * a_nm2;
        a_nm2  = a_nm1;
        a_nm1  = a_n;
    }
    return a_nm1;
}
