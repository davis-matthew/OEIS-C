/*
 * A070707  n^7 mod 26.
 * https://oeis.org/A070707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 26
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use repeated modular exponentiation to avoid overflow. */
    seq_t mod = (seq_t)26;
    seq_t base = (seq_t)n;
    seq_t result = (seq_t)1;
    uint64_t exp = 7ULL;          /* exponent 7 */

    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        exp >>= 1ULL;
        if (exp == 0ULL) break;
        base = (base * base) % mod;
    }
    return result;
}
