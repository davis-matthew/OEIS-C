/*
 * A075861  Least k such that (n-k) divides (n+k).
 * https://oeis.org/A075861
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n - max{ m | m < n and m divides 2n }  */
/* i.e., least k > 0 such that (n-k) divides (n+k) */

seq_t a(idx_t n)
{
    /* n is at least 2.  Compute 2n using a 64‑bit intermediate. */
    uint64_t two_n = 2 * (uint64_t)n;

    uint64_t best = 1;           /* 1 always divides 2n and is < n for n≥2 */
    uint64_t i;

    /* Scan all divisors of 2n up to sqrt(2n). */
    for (i = 1; i * i <= two_n; ++i)
    {
        if (two_n % i == 0)
        {
            uint64_t d1 = i;
            uint64_t d2 = two_n / i;

            if (d1 < n && d1 > best)
                best = d1;
            if (d2 < n && d2 > best)
                best = d2;
        }
    }

    /* The answer is k = n - best.  Convert to seq_t. */
    return (seq_t)(n - best);
}
