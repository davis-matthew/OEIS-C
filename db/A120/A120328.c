/*
 * A120328  Sum of three consecutive squares: a(n) = n^2 + (n + 1)^2 + (n +
 *          2)^2.
 * https://oeis.org/A120328
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n^2 + 6*n + 5
 * Range:     u8 n <= 8, u16 n <= 146, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    __int128 nn = n;                 /* signed 128‑bit intermediate */
    __int128 val = 3*nn*nn + 6*nn + 5;
    return (seq_t)val;               /* cast to the required seq_t type */
}
