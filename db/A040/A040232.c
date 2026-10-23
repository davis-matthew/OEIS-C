/*
 * A040232  Continued fraction for sqrt(248).
 * https://oeis.org/A040232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=15, for n>0 a(n) = 1 if n is odd; = 2 if n%4==2; = 30 if
 *            n%4==0
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)15;

    idx_t m = n;
    int v2 = 0;
    /* count the 2-adic valuation of n */
    while ((m & (idx_t)1) == (idx_t)0) {
        v2++;
        m >>= 1;
    }

    if (v2 == 0)          /* odd n */
        return (seq_t)1;
    else if (v2 == 1)     /* n ≡ 2 (mod 4) */
        return (seq_t)2;
    else                  /* n ≡ 0 (mod 4) */
        return (seq_t)30;
}
