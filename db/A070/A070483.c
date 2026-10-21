/*
 * A070483  a(n) = n^3 mod 21.
 * https://oeis.org/A070483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 21
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* reduce n modulo 21 to avoid overflow */
    unsigned long long r = n % 21ULL;
    unsigned long long sq = (r * r) % 21ULL;
    unsigned long long cube = (sq * r) % 21ULL;
    return (seq_t)cube;
}
