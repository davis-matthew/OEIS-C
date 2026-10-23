/*
 * A167298  Totally multiplicative sequence with a(p) = 6*(p-2) for prime p.
 * https://oeis.org/A167298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(1) = 1; a(2k) = 0 for k≥1; otherwise a(n) = ∏ₚ (6*(p−2))^eₚ(n)
 *            where the product runs over the distinct odd primes p dividing n
 *            and eₚ(n) is the exponent of p in the prime factorisation of n
 * Range:     u8 n <= 24, u16 n <= 624, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined behaviour, but harmless */
    if (n == 1) return 1;          /* a(1) = 1 */

    /* any even number greater than 1 gives 0 */
    if ((n & 1ULL) == 0)           /* n % 2 == 0 */
        return 0;

    seq_t prod = 1;
    idx_t tmp = n;

    /* factorisation by trial division; only odd factors remain */
    for (idx_t p = 3; p * p <= tmp; p += 2)
    {
        if (tmp % p == 0)
        {
            int exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                exp++;
            }

            seq_t factor = 1;
            seq_t mult = 6 * ((seq_t)p - 2);    /* 6*(p-2) */
            while (exp--)
                factor *= mult;

            prod *= factor;
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1)
    {
        seq_t mult = 6 * ((seq_t)tmp - 2);
        prod *= mult;
    }

    return prod;
}
