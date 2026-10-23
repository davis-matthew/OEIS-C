/*
 * A048691  a(n) = d(n^2), where d(k) = A000005(k) is the number of divisors
 *          of k.
 * https://oeis.org/A048691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(n) = d(n^2) = ∏_{p^e‖n} (2 e + 1)
 * Range:     u8 n <= 2519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;

    /* Trial‑division factorisation (fast enough for n ≤ 10 000) */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            uint64_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            result *= (seq_t)(2 * exp + 1);
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1)
        result *= (seq_t)3;   /* 2 · 1 + 1 */

    return result;
}
