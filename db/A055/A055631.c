/*
 * A055631  Sum of Euler's totient function phi of distinct primes dividing n.
 * https://oeis.org/A055631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p | n} (p - 1)
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0  (offset 1) */
    if (n < 2u)
        return 0;

    seq_t sum = 0;
    idx_t m = n;

    /* Trial division to extract distinct prime factors */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            sum += (seq_t)(p - 1);      /* add contribution of prime p   */
            while (m % p == 0)          /* strip all powers of p from m   */
                m /= p;
        }
    }

    /* If a prime factor larger than sqrt(original n) remains */
    if (m > 1)
        sum += (seq_t)(m - 1);

    return sum;
}
