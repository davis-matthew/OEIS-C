/*
 * A156821  Prime factors of 13! listed with multiplicity.
 * https://oeis.org/A156821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156821: prime factors of 13! listed with multiplicity (offset 1) */

seq_t a(idx_t n)
{
    /* Exponents of the primes in 13! */
    static const seq_t primes[]   = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
    /* 13! = 2^10 * 3^5 * 5^2 * 7^1 * 11^1 * 13^1 */
    static const idx_t exps[]     = {10, 5, 2, 1, 1, 1};

    /* Cumulative counts of factors up to each prime */
    static idx_t cumulative[7];
    static int initialized = 0;

    if (!initialized) {
        cumulative[0] = 0;
        for (int i = 0; i < 6; ++i)
            cumulative[i + 1] = cumulative[i] + exps[i];
        initialized = 1;
    }

    if (n < 1 || n > cumulative[6])
        return (seq_t)0;   /* undefined for out‑of‑range indices */

    for (int i = 0; i < 6; ++i)
        if (n <= cumulative[i + 1])
            return primes[i];

    return (seq_t)0;   /* safety, should not be reached */
}
