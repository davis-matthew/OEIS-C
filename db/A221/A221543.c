/*
 * A221543  Number of 0..n arrays of length 5 with each element differing from
 *          at least one neighbor by something other than 1, starting with 0.
 * https://oeis.org/A221543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + n^3 - 3*n^2 + 10*n - 9 for n > 3 Initial values:
 *            a(1)=3, a(2)=22, a(3)=103
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)22;
    if (n == 3) return (seq_t)103;
    /* n >= 4 */
    seq_t sn  = (seq_t)n;
    seq_t sn2 = sn * sn;        /* n^2 */
    seq_t sn3 = sn2 * sn;       /* n^3 */
    seq_t sn4 = sn3 * sn;       /* n^4 */
    return sn4 + sn3 - (seq_t)3 * sn2 + (seq_t)10 * sn - (seq_t)9;
}
