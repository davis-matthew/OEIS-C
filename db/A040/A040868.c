/*
 * A040868  Continued fraction for sqrt(898).
 * https://oeis.org/A040868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A040868
 *  a(0) = 29
 *  For n >= 1:
 *    let e = exponent of 2 in n (i.e. count of trailing zeros in binary)
 *    a(n) = 1          if e = 0 (n is odd)
 *    a(n) = 28         if e = 1 (n = 2 * odd)
 *    a(n) = 58         if e >= 2 (n divisible by 4)
 *  This follows the multiplicative rule: a(2)=28, a(2^e)=58 for e>=2,
 *  and a(p^e)=1 for odd primes p.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 29;   /* special offset term */

    /* count how many times 2 divides n */
    int e = 0;
    idx_t m = n;
    while ((m & 1ULL) == 0) {
        e++;
        m >>= 1;
    }

    if (e == 0)      return 1;   /* n odd */
    if (e == 1)      return 28;  /* n = 2 * odd */
    return 58;              /* n divisible by 4 */
}
