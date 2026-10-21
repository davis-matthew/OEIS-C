/*
 * A236327  a(n)*Pi is the total length of irregular spiral (center points: 1,
 *          2, 3, 4, 5; pattern 2) after n rotations.
 * https://oeis.org/A236327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 9*floor(n/4) + 9*floor((n+1)/4) + 4*floor((n+2)/4)
 * Range:     u8 n <= 34, u16 n <= 8738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) is undefined; return 0 for safety */

    idx_t q1 = n / 4;
    idx_t q2 = (n + 1) / 4;
    idx_t q3 = (n + 2) / 4;

    seq_t res = 0;
    res += (seq_t)2 * (seq_t)n;
    res += (seq_t)9 * (seq_t)q1;
    res += (seq_t)9 * (seq_t)q2;
    res += (seq_t)4 * (seq_t)q3;
    return res;
}
