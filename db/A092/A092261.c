/*
 * A092261  Sum of unitary, squarefree divisors of n, including 1.
 * https://oeis.org/A092261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of (p+1) for each prime p dividing n exactly once
 *            (i.e., where the exponent of p in n is 1). For n=1 the product
 *            is 1
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n < 1, but guard anyway */

    seq_t res = 1;          /* multiplicative identity */

    /* handle factor 2 separately to avoid checking odd numbers */
    idx_t exp = 0;
    while ((n & 1) == 0) {   /* n % 2 == 0 */
        n >>= 1;             /* n /= 2 */
        exp++;
    }
    if (exp == 1)
        res *= (seq_t)2 + 1;  /* multiply by (2+1) */

    /* trial division by odd numbers up to sqrt(n) */
    for (idx_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
            }
            if (exp == 1)
                res *= (seq_t)p + 1;   /* multiply by (p+1) */
        }
    }

    /* if n > 1, it is a prime factor left with exponent 1 */
    if (n > 1)
        res *= (seq_t)n + 1;

    return res;
}
