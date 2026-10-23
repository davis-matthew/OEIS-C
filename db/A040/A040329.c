/*
 * A040329  Continued fraction for sqrt(348).
 * https://oeis.org/A040329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=18; for n>0: * a(n)=1 if n is odd or n≡2 (mod 4), * a(n)=8
 *            if n≡4 (mod 8), * a(n)=36 if n≡0 (mod 8). This matches the
 *            stated multiplicative * definition (a(2)=1, a(4)=8, a(2^e)=36
 *            for e≥3, odd primes give 1)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)    /* special offset term */
        return (seq_t)18;

    /* count the exponent of 2 dividing n */
    idx_t e = 0;
    idx_t m = n;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }

    /* e is the 2‑adic valuation of n */
    if (e <= 1)        /* n odd (e==0) or n==2 (e==1) */
        return (seq_t)1;
    if (e == 2)        /* n divisible by 4 but not by 8 */
        return (seq_t)8;

    /* n divisible by 8 (e>=3) */
    return (seq_t)36;
}
