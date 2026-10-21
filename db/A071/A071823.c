/*
 * A071823  Number of numbers x <= n with largest prime factor of the form
 *          4k+3.
 * https://oeis.org/A071823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = # { x ≤ n | the largest prime factor of x is ≡ 3 (mod 4)
 *            }
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Count function required by the build */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                     /* indices below 1 are undefined */

    /* Allocate temporary arrays (C++ requires explicit cast) */
    idx_t *lpf  = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
    seq_t *cnt  = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));

    if (!lpf || !cnt)                      /* allocation failed */
    {
        abort();
    }

    /* Initialize */
    for (idx_t i = 0; i <= n; ++i)
        lpf[i] = 0;

    /* Sieve for largest prime factor */
    for (idx_t p = 2; p <= n; ++p)
    {
        if (lpf[p] == 0)                   /* p is prime */
        {
            for (idx_t m = p; m <= n; m += p)
                lpf[m] = p;                /* overwrite → largest factor finally stored */
        }
    }
    lpf[1] = 0;                             /* 1 has no prime factors */

    /* Prefix counts */
    cnt[0] = 0;
    if (n >= 1)
        cnt[1] = 0;                         /* a(1) = 0 */

    for (idx_t i = 2; i <= n; ++i)
    {
        cnt[i] = cnt[i - 1];
        if ((lpf[i] & 3ULL) == 3ULL)        /* check if largest prime factor ≡3 (mod4) */
        {
            cnt[i] = cnt[i] + (seq_t)1;
        }
    }

    seq_t result = cnt[n];

    /* Free temporary memory */
    free(lpf);
    free(cnt);

    return result;
}
