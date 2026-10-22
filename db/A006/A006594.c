/*
 * A006594  A Beatty sequence: floor(n*(1 + 1/e)).
 * https://oeis.org/A006594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * (1 + 1/e))
 * Range:     u8 n <= 187, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const unsigned __int128 c = 1367879441171442321ULL;
    const unsigned __int128 scale = 1000000000000000000ULL;  /* 10^18 */
    return (seq_t)(((__int128)n * c) / scale);
}
