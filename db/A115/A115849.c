/*
 * A115849  Integers i such that 17*i is not 17 X i.
 * https://oeis.org/A115849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer i such that (i >> 4) & i ≠ 0 (i.e. 17*i is
 *            not equal to carry‑less 17 X i)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for the offset 1 */
    seq_t i = 1;
    idx_t found = 0;
    for (;; ++i) {
        if (((i >> 4) & i) != 0) {
            ++found;
            if (found == n) return i;
        }
    }
}
