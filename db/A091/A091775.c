/*
 * A091775  a(n) = 1 + 4^n + 9^n + 16^n.
 * https://oeis.org/A091775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 4^n + 9^n + 16^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p4 = (seq_t)1;
    seq_t p9 = (seq_t)1;
    seq_t p16 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        p4  *= (seq_t)4;
        p9  *= (seq_t)9;
        p16 *= (seq_t)16;
    }
    seq_t result = (seq_t)1 + p4;
    result += p9;
    result += p16;
    return result;
}
