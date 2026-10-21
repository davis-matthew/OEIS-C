/*
 * A053692  Number of self-conjugate 4-core partitions of n.
 * https://oeis.org/A053692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1/2 * b(8*n + 5), where b(m) is multiplicative and for an
 *            odd prime p b(p^e) = (e+1) if p ≡ 1 (mod 4), b(p^e) = 1 if p ≡ 3
 *            (mod 4) and e even, b(p^e) = 0 if p ≡ 3 (mod 4) and e odd
 * Range:     u8 n <= 33, u16 n <= 8319, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* returns the number of self‑conjugate 4‑core partitions of n */
seq_t a(idx_t n)
{
    /* m = 8*n + 5 is always odd */
    seq_t m      = (seq_t)n * 8 + 5;
    seq_t prod   = 1;     /* product of (e+1) for primes ≡ 1 (mod 4) */
    seq_t temp   = m;

    /* trial division by odd numbers up to sqrt(temp) */
    for (seq_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            unsigned cnt = 0;
            while (temp % p == 0)
            {
                temp /= p;
                cnt++;
            }
            if (p % 4 == 3)          /* p ≡ 3 (mod 4) */
            {
                if (cnt & 1)          /* odd exponent → b = 0 */
                    return 0;
                /* even exponent contributes factor 1 – nothing to do */
            }
            else                     /* p ≡ 1 (mod 4) */
            {
                prod *= (seq_t)(cnt + 1);
            }
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1)
    {
        if (temp % 4 == 3)          /* odd exponent 1 ≡ 3 (mod 4) */
            return 0;
        /* temp ≡ 1 (mod 4) contributes factor (1+1) = 2 */
        prod *= 2;
    }

    /* a(n) = b(8*n + 5) / 2 */
    return prod / 2;
}
