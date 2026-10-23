/*
 * A040372  Continued fraction for sqrt(392).
 * https://oeis.org/A040372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A040372 – continued fraction coefficients for sqrt(392).
    a(0) = 19
    For n > 0:
        Let e = v2(n)  (exponent of 2 in n)
        a(n) = 1   if e = 0
        a(n) = 3   if e = 1
        a(n) = 38  if e ≥ 2
    This follows the multiplicative definition:
        a(p^e) = 1 for odd primes p,
        a(2) = 3, a(2^e) = 38 for e ≥ 2.
*/
seq_t a(idx_t n)
{
    if (n == 0)                    /* a(0) = floor(sqrt(392)) = 19 */
        return (seq_t)19;

    /* compute v2(n) */
    if ((n & 1ULL) == 0) {          /* n even => v2 ≥ 1 */
        unsigned e = 0;
        /* shift right until odd; n is at most 64 bits, loop cheap */
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        if (e == 1)
            return (seq_t)3;        /* a(2) = 3 */
        else
            return (seq_t)38;       /* a(2^e) with e ≥ 2 = 38 */
    } else {                         /* n odd */
        return (seq_t)1;             /* a(p^e) = 1 for odd p */
    }
}
