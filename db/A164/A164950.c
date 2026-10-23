/*
 * A164950  1 if there is a winning strategy for misère Sprouts with n initial
 *          points, else 0.
 * https://oeis.org/A164950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 1 for n > 4 and n ≡ 0,4,5 (mod 6); otherwise 0
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)0;
    /* n > 4 */
    idx_t r = n % (idx_t)6;
    if (r == (idx_t)0 || r == (idx_t)4 || r == (idx_t)5)
        return (seq_t)1;
    return (seq_t)0;
}
