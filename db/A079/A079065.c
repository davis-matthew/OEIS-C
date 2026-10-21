/*
 * A079065  In prime factorization of n replace odd primes with 3.
 * https://oeis.org/A079065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 2^{v2(n)} * 3^{sum of exponents of odd primes in n} (a is
 *            multiplicative, with a(2)=2 and a(p)=3 for odd primes)
 * Range:     u8 n <= 255, u16 n <= 9557, u32 n <= 9557, u64 n <= 9557,
 *            u128 n <= 9557, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 1; for n==0 we return 0 (behaviour below offset is irrelevant). */
    if (n == 0) return (seq_t)0;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* Handle factor 2 */
    while ((temp & 1) == 0) {         /* temp % 2 == 0 */
        result *= (seq_t)2;
        temp >>= 1;                  /* temp /= 2 */
    }

    /* Handle odd prime factors */
    if (temp > 1) {
        /* Trial division up to sqrt(temp) */
        for (idx_t p = 3; p <= temp / p; p += 2) {
            if (temp % p == 0) {
                /* Count exponent of prime p */
                uint64_t exp = 0;
                while (temp % p == 0) {
                    exp++;
                    temp /= p;
                }
                /* Multiply result by 3^exp */
                for (uint64_t i = 0; i < exp; ++i) {
                    result *= (seq_t)3;
                }
            }
        }
        /* If remaining temp > 1, it is an odd prime */
        if (temp > 1) {
            result *= (seq_t)3;   /* exponent 1 */
        }
    }

    return result;
}
