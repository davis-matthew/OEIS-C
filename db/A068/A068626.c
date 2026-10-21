/*
 * A068626  a(3n) = a(3n-1) = 3*n^2, a(3n-2) = 3*n^2 - 3*n + 1.
 * https://oeis.org/A068626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(3n) = a(3n-1) = 3*n^2, a(3n-2) = 3*n^2 - 3*n + 1
 * Range:     u8 n <= 27, u16 n <= 442, u32 n <= 9975, u64 n <= 9975,
 *            u128 n <= 9975, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the block index q such that n is in {3q-2, 3q-1, 3q} */
    idx_t q = (n + 2) / 3;           /* integer division, q >= 0 */
    seq_t q_s = (seq_t)q;
    seq_t base = 3 * q_s * q_s;     /* 3*q^2 */

    if (n % 3 == 1)                 /* n == 3q-2 */
        return base - 3 * q_s + 1;
    else
        return base;                /* n == 3q-1 or 3q      */
}
