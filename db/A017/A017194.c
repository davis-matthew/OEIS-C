/*
 * A017194  a(n) = (9*n + 2)^10.
 * https://oeis.org/A017194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 2)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 9, u128 n <= 792,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)9 * (seq_t)n) + (seq_t)2;
    seq_t res  = 1;
    for (int i = 0; i < 10; ++i) {
        res *= base;
    }
    return res;
}
