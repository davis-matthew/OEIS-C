/*
 * A197863  Smallest powerful number that is a multiple of n.
 * https://oeis.org/A197863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A197863: Smallest powerful number that is a multiple of n.
 * Multiplicative with a(p^e) = p^max(e,2).
 * a(n) = n * product_{p | n, e_p=1} p.
 */

#include <stdint.h> /* for uint64_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not defined for n=0, but safe */

    seq_t result = (seq_t)n;
    idx_t m = n;

    /* factor 2 separately */
    int cnt = 0;
    while ((m & 1ULL) == 0)
    {
        m >>= 1;
        cnt++;
    }
    if (cnt == 1)
        result *= (seq_t)2;

    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2)
    {
        if (m % p == 0)
        {
            cnt = 0;
            while (m % p == 0)
            {
                m /= p;
                cnt++;
            }
            if (cnt == 1)
                result *= (seq_t)p;
        }
    }

    /* remaining prime factor */
    if (m > 1)
        result *= (seq_t)m;

    return result;
}
