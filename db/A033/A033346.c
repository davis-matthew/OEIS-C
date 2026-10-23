/*
 * A033346  a(n) = floor(26/n).
 * https://oeis.org/A033346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(26/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Safety: the sequence is defined for n ≥ 1; return 0 for n == 0. */
    if (n == 0) {
        return (seq_t)0;
    }
    /* 26 is small enough to fit in any seq_t; perform division in
       unsigned 64-bit to avoid overflow when n > 26. */
    return (seq_t)(26ULL / n);
}
