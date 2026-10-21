/*
 * A092249  Positions of the integers in the standard diagonal enumeration of
 *          the rationals (with the integers in the first column and diagonals
 *          moving up to the right).
 * https://oeis.org/A092249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{i=2..n} φ(i) (φ = Euler's totient)
 * Range:     u8 n <= 28, u16 n <= 463, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;           /* undefined for n < 1, harmless */

    /* static cache: pref[i] holds a(i) for i ≤ pref_len */
    static idx_t *pref = NULL;
    static idx_t pref_len = 0;

    if (pref_len < n)                /* need to extend cache */
    {
        size_t size = (size_t)n + 1; /* array length = n+1 */

        /* temporary array for Euler's totient values */
        idx_t *phi = (idx_t *)malloc(size * sizeof(idx_t));
        if (!phi) abort();

        /* initialize phi[i] = i */
        for (size_t i = 0; i < size; ++i)
            phi[i] = (idx_t)i;

        /* sieve to compute φ up to n */
        for (size_t p = 2; p < size; ++p)
            if (phi[p] == (idx_t)p)                /* p is prime */
                for (size_t j = p; j < size; j += p)
                    phi[j] -= phi[j] / (idx_t)p;

        /* build prefix sums: a(1)=1, a(i)=a(i-1)+φ(i) for i≥2 */
        idx_t *new_pref = (idx_t *)malloc(size * sizeof(idx_t));
        if (!new_pref) abort();
        new_pref[0] = 0;
        if (size > 1) new_pref[1] = (idx_t)1;
        for (size_t i = 2; i < size; ++i)
            new_pref[i] = new_pref[i - 1] + phi[i];

        free(phi);

        /* replace old cache */
        free(pref);
        pref     = new_pref;
        pref_len = (idx_t)n;
    }

    return (seq_t)pref[n];
}
