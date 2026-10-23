/*
 * A183305  Half the number of nX4 binary arrays with no element equal to a
 *          strict majority of its horizontal and vertical neighbors.
 * https://oeis.org/A183305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementing OEIS A183305
   a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3) + a(n-4)
          - 9*a(n-5) + 4*a(n-6) + a(n-7) + 28*a(n-8)
          - 16*a(n-9) - 3*a(n-10) + 2*a(n-11) - 6*a(n-12)
          + 2*a(n-13) - a(n-14)   for n>15
   with initial values (n=1..15):
   2, 6, 19, 55, 178, 572, 1798, 5700, 18064, 57249,
   181433, 574924, 1821857, 5773450, 18295845
*/

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based array of computed terms */
static idx_t cachecap = 0;    /* highest index currently stored */

seq_t a(idx_t n)
{
    if (n == 0u) return 0;    /* undefined for offset 1, but return 0 */

    /* Ensure cache has room for indices up to n (1‑based) */
    if (cache == NULL) {
        size_t cap = (size_t)(n > 16u ? n : 16u);
        cache = (seq_t *)malloc((cap + 1u) * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failed */
        /* initialise first 15 terms */
        cache[1u] = (seq_t)2u;
        cache[2u] = (seq_t)6u;
        cache[3u] = (seq_t)19u;
        cache[4u] = (seq_t)55u;
        cache[5u] = (seq_t)178u;
        cache[6u] = (seq_t)572u;
        cache[7u] = (seq_t)1798u;
        cache[8u] = (seq_t)5700u;
        cache[9u] = (seq_t)18064u;
        cache[10u] = (seq_t)57249u;
        cache[11u] = (seq_t)181433u;
        cache[12u] = (seq_t)574924u;
        cache[13u] = (seq_t)1821857u;
        cache[14u] = (seq_t)5773450u;
        cache[15u] = (seq_t)18295845u;
        cachecap = (idx_t)cap;
    } else if (n > cachecap) {
        size_t newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (newcap + 1u) * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed */
        cache = tmp;
        cachecap = (idx_t)newcap;
    }

    if (n <= 15u) return cache[n];

    /* Compute terms up to n using the recurrence */
    for (idx_t i = 16u; i <= n; ++i) {
        seq_t pos = (seq_t)0u, neg = (seq_t)0u;

        pos += (seq_t)5u * cache[i - 1u];
        pos += cache[i - 3u];
        pos += cache[i - 4u];
        pos += (seq_t)4u * cache[i - 6u];
        pos += cache[i - 7u];
        pos += (seq_t)28u * cache[i - 8u];
        pos += (seq_t)2u * cache[i - 11u];
        pos += (seq_t)2u * cache[i - 13u];

        neg += (seq_t)6u * cache[i - 2u];
        neg += (seq_t)9u * cache[i - 5u];
        neg += (seq_t)16u * cache[i - 9u];
        neg += (seq_t)3u * cache[i - 10u];
        neg += (seq_t)6u * cache[i - 12u];
        neg += cache[i - 14u];

        cache[i] = pos - neg;   /* pos >= neg for all n */
    }

    return cache[n];
}
