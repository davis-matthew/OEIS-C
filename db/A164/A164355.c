/*
 * A164355  Expansion of (1 - x^2)^4 * (1 - x^5) / ((1 - x)^5 * (1 - x^4)^2)
 *          in powers of x.
 * https://oeis.org/A164355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, if n≡0 (mod 4) : a(n)=9*n/2 if n≡2 (mod 4) :
 *            a(n)=11*n/2 otherwise (n odd) : a(n)=5*n (This is the equivalent
 *            of a(n)=(20−((−i)^n+i^n))·n/4.)
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t r = n & 3ULL;              /* n mod 4 (fast mask) */
    seq_t ns = (seq_t)n;             /* convert once to seq_t */

    if (r == 0)          /* n ≡ 0 (mod 4) : n is a multiple of 4 */
        return ns * (seq_t)9 / (seq_t)2;
    if (r == 2)          /* n ≡ 2 (mod 4) : n is a multiple of 2 but not 4 */
        return ns * (seq_t)11 / (seq_t)2;

    /* n is odd (1 or 3 mod 4) */
    return ns * (seq_t)5;
}
