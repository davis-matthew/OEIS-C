/*
 * A070662  a(n) = n^6 mod 39.
 * https://oeis.org/A070662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 39
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 39 to keep intermediate values small */
    uint64_t m = n % 39ULL;
    uint64_t r = 1ULL;
    for (int i = 0; i < 6; ++i) {
        r = (r * m) % 39ULL;
    }
    return (seq_t)r;
}
