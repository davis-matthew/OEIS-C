/*
 * A055231  Powerfree part of n: product of primes that divide n only once.
 * https://oeis.org/A055231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of primes whose exponent in the prime
 *            factorisation of n is exactly 1 (the power‑free part of n)
 * Range:     u8 n <= 256, u16 n <= 9063, u32 n <= 9063, u64 n <= 9063,
 *            u128 n <= 9063, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the power‑free part of the integer n.  The result is
   always ≤ n, so the function works for any seq_t that can store n. */

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition;  n ≥ 1 here. */
    if (n <= 1)
        return 1;

    seq_t  res = 1;           /* accumulating product of primes with exp == 1 */
    idx_t  temp = n;          /* copy of n to factor */

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp == 1)          /* include the prime only once */
                res *= (seq_t)p;
        }
    }

    /* If a prime factor remains (temp > 1), its exponent is 1. */
    if (temp > 1)
        res *= (seq_t)temp;

    return res;
}
