/*
 * A010158  Continued fraction for sqrt(85).
 * https://oeis.org/A010158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0)=9; for n≥1, a(n)=p[(n-1) mod 5] where p={4,1,1,4,18}
 * Range:     u8 n <= 9082, u16 n <= 9082, u32 n <= 9082, u64 n <= 9082,
 *            u128 n <= 9082, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t block[5] = {4,1,1,4,18};
    if(n == 0)
        return 9;
    idx_t m = (n - 1) % 5;
    return block[m];
}
