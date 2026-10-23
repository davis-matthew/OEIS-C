/*
 * A010124  Continued fraction for sqrt(19).
 * https://oeis.org/A010124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=4; for n>0, a(n) = [2,1,3,1,2,8][(n-1)%6] (period 6)
 * Range:     u8 n <= 7741, u16 n <= 7741, u32 n <= 7741, u64 n <= 7741,
 *            u128 n <= 7741, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;
    idx_t r = (n - 1) % 6;
    switch (r) {
        case 0: return (seq_t)2;
        case 1: return (seq_t)1;
        case 2: return (seq_t)3;
        case 3: return (seq_t)1;
        case 4: return (seq_t)2;
        case 5: return (seq_t)8;
    }
    return (seq_t)0; /* unreachable */
}
