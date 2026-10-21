/*
 * A092259  Numbers that are congruent to {4, 8} mod 12.
 * https://oeis.org/A092259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n - 3 - (-1)^n (a(1)=4, a(2)=8, …)
 * Range:     u8 n <= 42, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    seq_t val = 6 * ((seq_t)n);   /* 6·n */
    if (n & 1)                    /* odd n → (-1)^n = –1, so a = 6*n–2 */
        val -= 2;
    else                          /* even n → (-1)^n = +1, so a = 6*n–4 */
        val -= 4;
    return val;
}
