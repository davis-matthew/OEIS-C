/*
 * A188669  a(n) = ceiling(binomial(2*n-1,n-1)/n).
 * https://oeis.org/A188669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(binomial(2*n-1,n-1)/n)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

extern seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* not used, safety */

    if (sizeof(seq_t) <= 8)            /* seq_t is at most 64‑bit */
    {
        unsigned __int128 prod = 1;    /* intermediate product   */
        idx_t           i;

        for (i = 1; i < n; ++i)
        {
            prod = prod * (unsigned __int128)(2*n - i);
            prod = prod / (unsigned __int128)i;
        }

        unsigned __int128 q128 = prod / (unsigned __int128)n;
        unsigned __int128 r128 = prod % (unsigned __int128)n;

        unsigned long long q = (unsigned long long)q128;
        unsigned long long r = (unsigned long long)r128;

        seq_t result = (seq_t)q;      /* safe cast to seq_t (<=64‑bit) */
        if (r > 0) result += 1;
        return result;
    }
    else                                 /* seq_t is arbitrary precision */
    {
        seq_t   prod = 1;
        idx_t i;

        for (i = 1; i < n; ++i)
            prod = prod * (seq_t)(2*n - i) / (seq_t)i;   /* exact big‑int ops */

        seq_t q = prod / (seq_t)n;
        seq_t r = prod % (seq_t)n;
        if (r > 0) q += 1;
        return q;
    }
}
