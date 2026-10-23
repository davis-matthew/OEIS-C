/*
 * A010211  Continued fraction for sqrt(162).
 * https://oeis.org/A010211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = 12 (if n = 0), else (n is even ? 2 : 1) * (n is multiple
 *            of 5 ? 12 : 1) This implements the multiplicative function
 *            described in OEIS A010211: a(2^e)=2, a(5^e)=12, a(p^e)=1 for odd
 *            primes p≠5
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)12;
    seq_t res = 1;
    if ((n & 1) == 0) res = (seq_t)2;        /* factor of 2 present */
    if (n % 5 == 0) res = res * (seq_t)12;    /* factor of 5 present */
    return res;
}
