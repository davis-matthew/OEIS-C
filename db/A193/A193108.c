/*
 * A193108  The tetrahedral numbers A000292 mod 10.
 * https://oeis.org/A193108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+2,3) mod 10
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n(n+1)(n+2)/6 modulo 10.
       Since 60 = 6 * 10, we can reduce the product modulo 60 first,
       then divide by 6 and take the result modulo 10. */
    seq_t nmod = (seq_t)(n % 60);
    seq_t m1   = (nmod + 1) % 60;  /* (n+1) mod 60 */
    seq_t m2   = (nmod + 2) % 60;  /* (n+2) mod 60 */

    seq_t prod = nmod;
    prod = (prod * m1) % 60;
    prod = (prod * m2) % 60;

    seq_t res = (prod / 6) % 10;   /* prod is divisible by 6 */
    return res;
}
