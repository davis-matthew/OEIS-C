/*
 * A055024  Number of 1-punctured staircase polygons (by perimeter) with a
 *          hole of perimeter 6.
 * https://oeis.org/A055024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*   n*(n-10)*(7922123*n -106081693)*a(n)
 * +2*(-100307698*n^3 +2258882361*n^2 -14191179053*n +19100093430)*a(n-1)
 * +16*(76541329*n^3 -1663392372*n^2 +11013283712*n -21512745186)*a(n-2)
 * -32*(n-7)*(34309603*n -256074474)*(2*n-13)*a(n-3) = 0
 * with a(0)=a(1)=...=a(9)=0, a(10)=2 */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cache of computed terms */
    static size_t len   = 0;       /* number of values populated */
    static size_t cap   = 0;       /* allocated capacity */

    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t k = len; k <= (size_t)n; ++k) {
        seq_t val;
        if (k <= 9)
            val = 0;
        else if (k == 10)
            val = 2;
        else {
            __int128 nn = k;

            /* leading coefficient A */
            __int128 A = nn * (nn - 10) *
                         ((__int128)7922123 * nn - (__int128)106081693);

            /* coefficient for a(n‑1) */
            __int128 B = 2 * ((-(__int128)100307698) * nn * nn * nn
                              + (__int128)2258882361LL * nn * nn
                              - (__int128)14191179053LL * nn
                              + (__int128)19100093430LL);

            /* coefficient for a(n‑2) */
            __int128 C = 16 * ((__int128)76541329 * nn * nn * nn
                               - (__int128)1663392372LL * nn * nn
                               + (__int128)11013283712LL * nn
                               - (__int128)21512745186LL);

            /* coefficient for a(n‑3) */
            __int128 D = - (__int128)32 *
                         (nn - 7) *
                         ((__int128)34309603LL * nn - (__int128)256074474LL) *
                         (2 * nn - 13);

            /* numerator: -(B*a(n‑1)+C*a(n‑2)+D*a(n‑3)) */
            __int128 num = - (
                (__int128)B * (__int128)cache[k-1] +
                (__int128)C * (__int128)cache[k-2] +
                (__int128)D * (__int128)cache[k-3]
            );

            val = (seq_t)(num / A);
        }
        cache[k] = val;
    }

    len = (size_t)n + 1;
    return cache[n];
}
