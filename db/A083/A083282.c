/*
 * A083282  a(n) = n^(3*n).
 * https://oeis.org/A083282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(3*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset; the sequence is defined for n >= 1.
       For n == 0 we return 0 (unused but harmless). */
    if (n == 0) return (seq_t)0;

    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)n;
    unsigned __int128 exp = (unsigned __int128)n * 3;

    while (exp != 0) {
        if (exp & 1)
            result = result * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }
    return result;
}
