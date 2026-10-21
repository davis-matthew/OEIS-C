/*
 * A062004  a(n) = 2*n*mu(n).
 * https://oeis.org/A062004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*n*mu(n) (mu is the Möbius function)
 * Range:     i8 n <= 64, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; 0 is never requested */

    idx_t temp = n;                /* copy of n for factorisation */
    int  odd   = 0;                /* will be toggled for each distinct prime */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            int expo = 0;
            while (temp % p == 0)
            {
                ++expo;
                temp /= p;
                if (expo > 1)          /* repeated prime factor → mu = 0 */
                    return 0;
            }
            odd ^= 1;                  /* toggle parity for this prime factor */
        }
    }
    if (temp > 1)                      /* remaining prime factor > sqrt(original n) */
        odd ^= 1;

    seq_t mu = odd ? (seq_t)(-1) : (seq_t)1;     /* mu(n) */
    return (seq_t)2 * (seq_t)n * mu;             /* a(n) = 2*n*mu(n) */
}
