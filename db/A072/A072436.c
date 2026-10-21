/*
 * A072436  Remove prime factors of form 4*k+3.
 * https://oeis.org/A072436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product of all prime powers p^e dividing n where p == 2
 *            or p % 4 == 1 ; factors congruent to 3 mod 4 are removed
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)  /* a(1) = 1 */
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* factor 2, multiply all its powers */
    idx_t e = 0;
    while (m % 2 == 0) {
        m /= 2;
        ++e;
    }
    for (idx_t i = 0; i < e; ++i)
        result *= (seq_t)2;

    /* factor odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (p % 4 == 1) {   /* only keep primes ≡ 1 (mod 4) */
                for (idx_t i = 0; i < exp; ++i)
                    result *= (seq_t)p;
            }
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        if (m % 4 == 1)      /* m is prime ≡ 1 (mod 4) */
            result *= (seq_t)m;
        /* else it is ≡ 3 (mod 4) and discarded */
    }

    return result;
}
