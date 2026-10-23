/*
 * A185450  Define a map of the integers by T_k(i)=i/2 if i even, (3i+k)/2 if
 *          i odd; a(n) is the conjectured number of cycles for trajectories
 *          under this map in the case k = 5^n.
 * https://oeis.org/A185450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 (n=0), 10 (n=1), 13 (n=2), 2*n + 11 (n≥3)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 5;
    if (n == 1) return 10;
    if (n == 2) return 13;
    return (seq_t)2 * n + (seq_t)11;
}
