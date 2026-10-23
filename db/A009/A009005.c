/*
 * A009005  All natural numbers except 1, 2 and 4.
 * https://oeis.org/A009005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 3; a(n) = n + 3 for n >= 2 (all natural numbers except 1,
 *            2, 4)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is uint64_t; cast to seq_t before arithmetic to avoid overflow */
    if (n == (idx_t)1) {
        return (seq_t)3;
    } else {
        return (seq_t)n + (seq_t)3;
    }
}
