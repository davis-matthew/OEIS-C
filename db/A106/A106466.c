/*
 * A106466  Interleave 1,2,3,.. with 1,1,2,2,3,3,...
 * https://oeis.org/A106466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+2)/2 if n is even; (n+3)/4 if n ≡ 1 (mod 4); (n+1)/4
 *            if n ≡ 3 (mod 4)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n + (seq_t)2;  /* s = n + 2 */
    if ((n & 1) == 0)              /* n even */
        return s / 2;
    else if ((n & 3) == 1)         /* n ≡ 1 (mod 4) */
        return (s + (seq_t)1) / 4;
    else                           /* n ≡ 3 (mod 4) */
        return (s - (seq_t)1) / 4;
}
