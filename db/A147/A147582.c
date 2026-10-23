/*
 * A147582  First differences of A147562.
 * https://oeis.org/A147582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = 4 * 3^(popcount(n-1) - 1)
 * Range:     u8 n <= 31, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return 1;
    }

    /* popcount of n-1 (n > 1 guarantees n-1 ≥ 1) */
    unsigned int bits = __builtin_popcountll((unsigned long long)(n - 1));
    unsigned int exp = bits - 1;  /* exponent for 3 */

    seq_t val = 1;
    for (unsigned int i = 0; i < exp; ++i) {
        val *= 3;
    }
    val *= 4;  /* multiply by 4 at the end */

    return val;
}
