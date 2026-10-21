/*
 * A070699  a(n) = n^7 mod 17.
 * https://oeis.org/A070699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 17
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)(n % 17);
    seq_t res  = 1;
    for (int i = 0; i < 7; ++i) {
        res = (res * base) % (seq_t)17;
    }
    return res;
}
