/*
 * A090020  Number of distinct lines through the origin in the n-dimensional
 *          lattice of side length 4.
 * https://oeis.org/A090020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n - 3^n - 2^n + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the O(n) exponentiation algorithm for arbitrary seq_t types. */

static seq_t power_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    seq_t p5 = power_seq_t((seq_t)5, n);
    seq_t p3 = power_seq_t((seq_t)3, n);
    seq_t p2 = power_seq_t((seq_t)2, n);
    return p5 - p3 - p2 + (seq_t)1;
}
