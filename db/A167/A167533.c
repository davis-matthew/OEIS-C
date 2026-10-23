/*
 * A167533  a(n) = 71*n - a(n-1) for n>0, a(0)=12.
 * https://oeis.org/A167533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 12; a(n) = 71*n - a(n-1) for n > 0
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {
        seq_t half = (seq_t)(n / 2);
        return (seq_t)12 + (seq_t)71 * half;
    } else {
        seq_t half = (seq_t)((n - 1) / 2);
        return (seq_t)59 + (seq_t)71 * half;
    }
}
