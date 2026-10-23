/*
 * A040549  Continued fraction for sqrt(573).
 * https://oeis.org/A040549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0)=23; for n≥1, a(n)=1 if n is odd, a(n)=14 if n divisible by
 *            2 but not by 4, a(n)=46 if n divisible by 4 (i.e., a(2)=14,
 *            a(2^e)=46 for e≥2, a(p^e)=1 for odd primes)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;                 /* floor(sqrt(573)) = 23 */

    /* compute the exponent of 2 dividing n */
    idx_t k = 0;
    idx_t t = n;
    while ((t & (idx_t)1) == 0) {
        t >>= 1;
        k++;
    }

    if (k == 0)
        return (seq_t)1;          /* n odd */
    else if (k == 1)
        return (seq_t)14;         /* n divisible by 2 only once */
    else
        return (seq_t)46;         /* n divisible by 4 (2^e, e≥2) */
}
