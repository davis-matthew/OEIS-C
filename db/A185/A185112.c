/*
 * A185112  a(1) = 0; for n > 1, a(n) = a(n-1) - P(n) if that number is zero
 *          or positive, otherwise a(n) = a(n-1) + P(n), where P(n) is the
 *          largest prime factor of n (A006530).
 * https://oeis.org/A185112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 172, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A185112
   a(1) = 0
   For n > 1 let P(n) be the largest prime factor of n.
   If a(n‑1) – P(n) ≥ 0, then a(n) = a(n‑1) – P(n),
   otherwise a(n) = a(n‑1) + P(n). */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;                 /* a(1) = 0, undefined below 1 */

    /* largest prime factor array 1‑based, size n+1 */
    uint64_t *lpf = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!lpf)
        return (seq_t)0;                 /* allocation failure – undefined behaviour */

    memset(lpf, 0, (size_t)(n + 1) * sizeof(uint64_t));

    /* sieve for largest prime factor (overwriting with larger primes) */
    for (idx_t p = 2; p <= n; ++p)
    {
        if (lpf[p] == 0)                 /* p is prime */
        {
            for (idx_t j = p; j <= n; j += p)
                lpf[j] = p;              /* overwrite – keeps largest prime factor */
        }
    }

    seq_t val = (seq_t)0;                 /* a(1) */

    for (idx_t k = 2; k <= n; ++k)
    {
        uint64_t pk = lpf[k];
        if (val >= (seq_t)pk)
            val -= (seq_t)pk;
        else
            val += (seq_t)pk;
    }

    free(lpf);
    return val;
}
