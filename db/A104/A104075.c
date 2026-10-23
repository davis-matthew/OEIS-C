/*
 * A104075  Repdigits, the squares of which are palindromes.
 * https://oeis.org/A104075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  full, nonn, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A104075: Repdigits whose squares are palindromes.
   a(0)=0, a(1)=1, a(2)=2, a(3)=3, a(4)=11, a(5)=22,
   and for n≥6  a(n) = (10^{n-3} – 1)/9  (a repunit). */

seq_t a(idx_t n)
{
    /* explicit small values */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 3;
    if (n == 4) return 11;
    if (n == 5) return 22;

    /* n ≥ 6 : build a repunit of length n‑3 */
    idx_t len = n - 3;
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i)
        val = val * 10 + 1;
    return val;
}
