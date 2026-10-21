/*
 * A070649  a(n) = n^6 mod 26.
 * https://oeis.org/A070649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 26
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n % 26;
    seq_t res = 1;
    for (int i = 0; i < 6; ++i) {
        res = (res * base) % 26;
    }
    return res;
}
