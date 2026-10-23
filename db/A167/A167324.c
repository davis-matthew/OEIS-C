/*
 * A167324  Totally multiplicative sequence with a(p) = 5*(p+3) for prime p.
 * https://oeis.org/A167324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏(5*(p+3))^e over all prime powers p^e in n's
 *            factorisation
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 1 by convention */
    if (n == 1)
        return (seq_t)1;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;       /* factorise using 64‑bit arithmetic */

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t term = (seq_t)(5ULL * (p + 3ULL));
            for (unsigned i = 0; i < exp; ++i)
                result *= term;
        }
    }

    /* if m > 1, it is a prime factor left over */
    if (m > 1) {
        seq_t term = (seq_t)(5ULL * (m + 3ULL));
        result *= term;
    }

    return result;
}
