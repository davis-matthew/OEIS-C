/*
 * A057577  Maximal numbers of codewords in mixed code with n binary
 *          coordinates and 4 ternary coordinates with Hamming distance 3.
 * https://oeis.org/A057577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 9·2^(n−1) for n ≥ 4; initial values: a(0) = 9, a(1) = 12,
 *            a(2) = 22, a(3) = 42
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle tiny n explicitly. */
    if (n == 0) return (seq_t)9;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)22;
    if (n == 3) return (seq_t)42;

    /* For n ≥ 4: a(n) = 9 * 2^(n-1) */
    seq_t res = (seq_t)9;
    for (idx_t i = 1; i < n; ++i)
        res = res * (seq_t)2;   /* multiply by 2 (n-1) times */
    return res;
}
