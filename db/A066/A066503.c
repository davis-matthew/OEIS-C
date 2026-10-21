/*
 * A066503  a(n) = n - squarefree kernel of n, A007947.
 * https://oeis.org/A066503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - squarefree kernel of n (product of distinct prime
 *            factors)
 * Range:     u8 n <= 287, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            // undefined for offset 0, but safe
    seq_t rad = 1;
    idx_t temp = n;

    /* factor out 2 once */
    if ((temp & 1) == 0) {
        rad *= (seq_t)2;
        while ((temp & 1) == 0) temp >>= 1;
    }

    /* trial division by odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            rad *= (seq_t)p;
            while (temp % p == 0) temp /= p;
        }
    }

    /* remaining prime factor > sqrt(temp) */
    if (temp > 1) {
        rad *= (seq_t)temp;
    }

    return (seq_t)n - rad;
}
