/*
 * A015053  Smallest positive integer for which n divides a(n)^6.
 * https://oeis.org/A015053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over prime powers p^e||n of p^ceil(e/6)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the smallest positive integer a such that n divides a^6. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    seq_t result = 1;
    idx_t temp = n;

    /* Count factor 2 separately for speed */
    idx_t e2 = 0;
    while ((temp & 1ULL) == 0) {
        e2++;
        temp >>= 1;
    }
    if (e2) {
        idx_t pow2 = (e2 + 5) / 6;           /* ceil(e2/6) */
        seq_t mult = 1;
        for (idx_t i = 0; i < pow2; ++i)
            mult *= (seq_t)2;
        result *= mult;
    }

    /* Handle odd prime factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                exp++;
                temp /= p;
            }
            idx_t powp = (exp + 5) / 6;        /* ceil(exp/6) */
            seq_t factor = 1;
            for (idx_t i = 0; i < powp; ++i)
                factor *= (seq_t)p;
            result *= factor;
        }
    }

    /* If a prime factor remains */
    if (temp > 1) {
        /* Remaining prime has exponent 1 -> ceil(1/6)=1 */
        result *= (seq_t)temp;
    }

    return result;
}
