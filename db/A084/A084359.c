/*
 * A084359  a(n) = number of partitions of n into pair of parts n=p+q,
 *          p>=q>=1, with p-q equal to a square >= 0.
 * https://oeis.org/A084359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k, 1 <= k <= n/2 such that n-2k is a
 *            square
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    /* Iterate over square roots a such that a*a <= n. */
    for (idx_t a = 0; /* a <= sqrt(n) handled in the loop */; ++a)
    {
        unsigned __int128 prod = (unsigned __int128)a * a;
        if (prod > (unsigned __int128)n) break;
        idx_t s = (idx_t)prod;                /* square value (a*a) */
        idx_t diff = n - s;                   /* n - a^2 */
        if ((diff & 1ULL) != 0) continue;     /* parity must match */
        idx_t k = diff / 2;                   /* corresponding k */
        if (k < 1ULL || k > n / 2) continue;
        ++cnt;
    }
    return cnt;
}
