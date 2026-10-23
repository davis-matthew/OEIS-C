/*
 * A171813  Trajectory of 1 under repeated applications of the map i ->
 *          A171798(i).
 * https://oeis.org/A171813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; a(2)=101; a(3)=734; a(4)=1037; a(5)=1174; a(n)=1165 for
 *            n≥6
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)101;
    if (n == 3)
        return (seq_t)734;
    if (n == 4)
        return (seq_t)1037;
    if (n == 5)
        return (seq_t)1174;
    return (seq_t)1165;
}
