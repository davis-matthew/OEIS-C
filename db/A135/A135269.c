/*
 * A135269  Row 5 of A038207.
 * https://oeis.org/A135269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, nonn, full
 * Formula:   a(n) = binom(5,n) * 2^(5-n) for 0 ≤ n ≤ 5, otherwise 0
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 5) return 0;
    static const seq_t binoms[6] = {1, 5, 10, 10, 5, 1};
    seq_t comb   = binoms[n];
    idx_t exp    = 5 - n;               /* 0 ≤ exp ≤ 5 */
    seq_t pow2   = (seq_t)(1ULL << exp); /* 2^(5-n) */
    return comb * pow2;
}
