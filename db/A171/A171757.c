/*
 * A171757  Even numbers whose binary expansion begins 10.
 * https://oeis.org/A171757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*A004761(n+1) (even numbers whose binary expansion
 *            begins 10)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t T = 1;
    while (T < (seq_t)n)
        T <<= 1;                /* T = smallest power of two ≥ n */
    seq_t half = T >> 1;
    return (T << 1) + ((seq_t)n - half - (seq_t)1) * 2;
}
