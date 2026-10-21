/*
 * A236326  a(n)*Pi is the total length of irregular spiral (center points: 1,
 *          2, 3, 4, 5; pattern 1) after n rotations.
 * https://oeis.org/A236326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + 4*floor(n/5) + 4*floor((n+1)/5) + floor((n+2)/5)
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;               /* cast index to seq_t for calculations */
    seq_t t1 = nn / 5;          /* floor(n/5)   */
    seq_t t2 = (nn + 1) / 5;    /* floor((n+1)/5) */
    seq_t t3 = (nn + 2) / 5;    /* floor((n+2)/5) */
    return 3 * nn + 4 * t1 + 4 * t2 + t3;
}
