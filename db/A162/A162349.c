/*
 * A162349  First differences of A160412.
 * https://oeis.org/A162349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(1 + popcount(n-1))
 * Range:     u8 n <= 31, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count the number of 1 bits in n-1 */
    idx_t x = n - 1;
    idx_t pc = 0;
    while (x) {
        x &= x - 1;
        ++pc;
    }
    /* Compute 3^(pc+1) */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < pc + 1; ++i) {
        result = result * (seq_t)3;
    }
    return result;
}
