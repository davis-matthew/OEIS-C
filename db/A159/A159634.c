/*
 * A159634  Coefficient for dimensions of spaces of modular & cusp forms of
 *          weight k/2, level 4*n and trivial character, where k>=5 is odd.
 * https://oeis.org/A159634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, mult
 * Formula:   a(n) = n*∏_{p odd|n} (1 + 1/p)
 * Range:     u8 n <= 164, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A159634: coefficient for dimensions of spaces of modular & cusp forms
   of weight k/2, level 4*n and trivial character. */
seq_t a(idx_t n)
{
    /* Start with the value n itself, then multiply by (p+1)/p for each
       distinct odd prime divisor p of n.  The division is exact because
       n contains at least one factor p, so after multiplying by (p+1)
       we can safely divide by p once. */
    seq_t res = (seq_t)n;

    /* Reduce n to its odd part (remove all powers of 2). */
    idx_t m = n;
    while ((m & 1ULL) == 0)
        m >>= 1;

    /* Iterate over odd numbers as potential prime factors. */
    for (idx_t p = 3; p * p <= m; p += 2)
    {
        if (m % p == 0)
        {
            /* product over distinct odd primes */
            res = res * (seq_t)(p + 1) / (seq_t)p;
            /* remove all powers of p from m */
            while (m % p == 0)
                m /= p;
        }
    }

    /* If a prime factor > sqrt(original m) remains, it is a single prime. */
    if (m > 1)
        res = res * (seq_t)(m + 1) / (seq_t)m;

    return res;
}
