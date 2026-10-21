/*
 * A214055  Least m>0 such that n!+2+m and n-m have a common divisor > 1.
 * https://oeis.org/A214055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m>0 such that (n!+2+m) and (n-m) have a common
 *            divisor >1. After analysis one finds that a(n) equals the
 *            smallest integer m = p-2 (p=2 gives m=2) over all prime divisors
 *            p of (n+2) that satisfy p ≤ n-1; if there is no such prime, then
 *            a(n) = n
 * Range:     u8 n <= 260, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* default candidate is n itself */
    idx_t best = n;

    /* n+2 is the number whose prime factors are relevant */
    idx_t T = n + (idx_t)2;
    /* handle the prime factor 2 */
    if (n >= (idx_t)2 && (T & (idx_t)1) == 0) /* T % 2 == 0 */
        best = (idx_t)2;

    /* remove all factors of 2 from T for the odd factor search */
    idx_t tmp = T;
    while ((tmp & (idx_t)1) == 0) /* tmp % 2 == 0 */
        tmp /= 2;

    /* trial divide by odd candidates up to sqrt(tmp) */
    for (idx_t p = 3; p * p <= tmp; p += 2)
    {
        if (tmp % p == 0) /* p is a prime factor of original T */
        {
            /* remove all powers of this prime */
            while (tmp % p == 0)
                tmp /= p;

            if (p <= n - (idx_t)1)
            {
                idx_t cand = (p == (idx_t)2) ? (idx_t)2 : (p - (idx_t)2);
                if (cand < best)
                    best = cand;
            }
        }
    }

    /* if anything remains, it is a prime factor > sqrt(T) */
    if (tmp > (idx_t)1 && tmp <= n - (idx_t)1)
    {
        idx_t cand = (tmp == (idx_t)2) ? (idx_t)2 : (tmp - (idx_t)2);
        if (cand < best)
            best = cand;
    }

    return (seq_t)best;
}
