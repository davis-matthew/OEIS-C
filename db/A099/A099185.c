/*
 * A099185  Iterated octahedral numbers, starting at a(0) = 2.
 * https://oeis.org/A099185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(0)=2; a(n+1) = (2*a(n)^3 + a(n))/3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Start with a(0) */
    seq_t val = (seq_t)2;
    if (n == 0) return val;
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute (2*val^3 + val) / 3 */
        /* Using left associativity to keep intermediate types the same */
        seq_t temp = 2 * val * val * val + val;
        val = temp / 3;
    }
    return val;
}
