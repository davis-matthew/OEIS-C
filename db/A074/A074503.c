/*
 * A074503  a(n) = 1^n + 2^n + 7^n.
 * https://oeis.org/A074503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 2^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    seq_t term1 = power_seq_t(1, n); /* always 1, kept for clarity */
    seq_t term2 = power_seq_t(2, n);
    seq_t term3 = power_seq_t(7, n);
    return term1 + term2 + term3;
}
