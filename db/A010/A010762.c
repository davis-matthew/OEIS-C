/*
 * A010762  a(n) = floor(n/2) * floor(n/3).
 * https://oeis.org/A010762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) * floor(n/3)
 * Range:     u8 n <= 39, u16 n <= 627, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t early to preserve type safety */
    seq_t sn = (seq_t)n;
    seq_t half = sn / (seq_t)2;
    seq_t third = sn / (seq_t)3;
    return half * third;
}
