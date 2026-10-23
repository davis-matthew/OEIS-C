/*
 * A166698  Totally multiplicative sequence with a(p) = a(p-1) - 1 for prime
 *          p.
 * https://oeis.org/A166698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = 0 if n is even, a(n) = 1 if n is odd and bigomega(n) is
 *            even, a(n) = -1 if n is odd and bigomega(n) is odd
 * Range:     i8 n <= 9978, i16 n <= 9978, i32 n <= 9978, i64 n <= 9978,
 *            i128 n <= 9978, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even numbers give zero immediately. */
    if ((n & 1) == 0)
        return (seq_t)0;

    /* Count prime factors with multiplicity (Ω(n)). */
    idx_t tmp = n;
    idx_t cnt = 0;

    /* Trial division by odd primes only. */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        while (tmp % p == 0) {
            cnt++;
            tmp /= p;
        }
    }
    if (tmp > 1)  /* remaining prime factor */
        cnt++;

    /* Return sign based on parity of count. */
    return (cnt % 2 == 0) ? (seq_t)1 : (seq_t)(-1);
}
