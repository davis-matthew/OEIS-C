/*
 * A170956  Expansion of Product_{i=1..m} (1 + x^(4*i-1)) for m = 3.
 * https://oeis.org/A170956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 if n∈{0,3,7,10,11,14,18,21}, else 0
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Exponents that appear in (1 + x^3)(1 + x^7)(1 + x^11). */
    static const idx_t exps[8] = {0, 3, 7, 10, 11, 14, 18, 21};
    for (idx_t i = 0; i < 8; ++i) {
        if (n == exps[i]) {
            return (seq_t)1;
        }
    }
    return (seq_t)0;
}
