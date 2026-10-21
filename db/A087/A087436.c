/*
 * A087436  Number of odd prime factors of n, counted with repetitions.
 * https://oeis.org/A087436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of odd prime factors of n, counted with
 *            multiplicity
 * Range:     u8 n <= 9688, u16 n <= 9688, u32 n <= 9688, u64 n <= 9688,
 *            u128 n <= 9688, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0, and for all n ≥ 2 the count is computed by trial division. */
    if (n <= 1) return 0;

    idx_t m = n;

    /* Remove factor 2 completely – it is not counted. */
    while ((m & 1ULL) == 0) m >>= 1;
    if (m == 1) return 0;          /* n was a power of two */

    seq_t cnt = 0;

    /* Trial division by odd numbers up to sqrt(m). */
    for (idx_t p = 3; p <= m / p; p += 2)
    {
        while (m % p == 0)
        {
            m /= p;
            ++cnt;
        }
        if (m == 1) break;
    }

    /* Any remaining m > 1 is an odd prime. */
    if (m > 1) ++cnt;

    return cnt;
}
