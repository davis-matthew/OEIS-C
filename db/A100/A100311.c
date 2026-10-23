/*
 * A100311  Modulo 2 binomial transform of 8^n.
 * https://oeis.org/A100311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A100311: Modulo 2 binomial transform of 8^n
   Recurrence (conjecture):  a(0) = 1;
                            a(n+1) = (a(n) << 3) ^ a(n);   // multiply by 8, then XOR with the previous value
   This implements a(n) = Σ_{k=0..n} (C(n,k) mod 2) * 8^k
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t cur = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        cur = (cur << 3) ^ cur;
    return cur;
}
