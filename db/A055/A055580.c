/*
 * A055580  Björner-Welker sequence: 2^n*(n^2 + n + 2) - 1.
 * https://oeis.org/A055580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 2^n * (n^2 + n + 2) - 1
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n in a loop (sequence length up to ~10000) */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 = pow2 * (seq_t)2;

    seq_t n_seq = (seq_t)n;
    seq_t inner = n_seq * n_seq + n_seq + (seq_t)2;
    seq_t res   = pow2 * inner - (seq_t)1;
    return res;
}
