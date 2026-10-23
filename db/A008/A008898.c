/*
 * A008898  Trajectory of 84 under the map x -> x/2 for x even, x -> 3x - 1
 *          for x odd.
 * https://oeis.org/A008898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=84; for n>0, a(n) = a(n-1)/2 if a(n-1) even, else
 *            3*a(n-1)-1
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)84;
    for (idx_t i = 0; i < n; ++i) {
        if ((x & (seq_t)1) == 0)
            x >>= 1;
        else
            x = x * (seq_t)3 - (seq_t)1;
    }
    return x;
}
