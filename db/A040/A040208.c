/*
 * A040208  Continued fraction for sqrt(223).
 * https://oeis.org/A040208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 14; For n > 0 : a(n) = 13 if n ≡ 2 (mod 4), 28 if n ≡ 0
 *            (mod 4), 1 otherwise (equivalently, multiplicative with a(2)=13,
 *            a(2^e)=28 for e ≥ 2, and a(p^e)=1 for all odd primes p)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)14;

    /* For all odd n, the value is 1. */
    if ((n & 1) == 1) return (seq_t)1;

    /* n is even.  Distinguish whether the exponent of 2 is 1 or ≥2. */
    if ((n & 3) == 2)            /* n ≡ 2 (mod 4) → exponent of 2 is exactly 1 */
        return (seq_t)13;
    else                          /* n ≡ 0 (mod 4) → exponent of 2 is at least 2 */
        return (seq_t)28;
}
