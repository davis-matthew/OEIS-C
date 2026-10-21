/*
 * A070724  n^7 mod 44.
 * https://oeis.org/A070724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 44
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t mod_n = n % 44;
    seq_t base = (seq_t)mod_n;
    idx_t prod = 1;
    for (int i = 0; i < 7; ++i) {
        prod = (prod * (idx_t)base) % 44;
    }
    return (seq_t)prod;
}
