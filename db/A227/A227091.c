/*
 * A227091  Number of solutions to x^2 == 1 (mod n) in Z[i]/nZ[i].
 * https://oeis.org/A227091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) is multiplicative. For n>1 : a(n)=∏ a(p^e) where
 *            a(2^e)=2^min(e,3), a(p^e)=4 if p≡1 (mod 4), a(p^e)=2 if p≡3 (mod
 *            4). a(1)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of solutions of x^2≡1 (mod n) in Z[i]/nZ[i] */
seq_t a(idx_t n)
{
    if (n == 0)    /* not used in OEIS but guard anyway */
        return (seq_t)0;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* handle the factor 2 separately */
    int e2 = 0;
    while ((m & 1ULL) == 0) {
        e2++;
        m >>= 1;
    }
    if (e2 > 0) {
        if (e2 >= 3) {
            result *= (seq_t)8;            /* 2^3 */
        } else {
            result *= (seq_t)(1U << e2);   /* 2,4 */
        }
    }

    /* odd primes up to sqrt(m) */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                exp++;
                m /= p;
            }
            /* exponent exp is irrelevant for the value */
            if (p % 4 == 1)
                result *= (seq_t)4;
            else
                result *= (seq_t)2;
        }
    }

    /* any remaining prime factor > sqrt(original n) */
    if (m > 1) {
        if (m % 4 == 1)
            result *= (seq_t)4;
        else
            result *= (seq_t)2;
    }

    return result;
}
