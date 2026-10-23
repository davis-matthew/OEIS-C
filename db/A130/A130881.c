/*
 * A130881  Numbers m such that digitsum((m+k)*abs(m-k)) = m for some k >= 0.
 * https://oeis.org/A130881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=0, a(2)=1, a(3)=8, a(4)=9, a(5)=10. For n ≥ 6 let i = n−6.
 *            k = 2 + ⌊i/2⌋ (integer division). If i is even → a(n) = 9·k − 1.
 *            If i is odd → a(n) = 9·k. This reproduces the OEIS sequence
 *            A130881
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 8;
    if (n == 4) return 9;
    if (n == 5) return 10;

    idx_t i    = n - 6;                 /* i ≥ 0 */
    seq_t k    = (seq_t)2 + (seq_t)(i / 2);
    seq_t term;
    if ((i & 1) == 0)                  /* i even */
        term = k * 9 - 1;
    else                               /* i odd */
        term = k * 9;
    return term;
}
