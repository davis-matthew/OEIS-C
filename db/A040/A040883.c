/*
 * A040883  Continued fraction for sqrt(914).
 * https://oeis.org/A040883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction terms of sqrt(914). a(0)=30; for n>=1, a(n)
 *            repeats pattern 4,3,3,4,60
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)30u;
    idx_t r = (n - 1) % 5;
    static const int pat[5] = {4, 3, 3, 4, 60};
    return (seq_t)pat[r];
}
