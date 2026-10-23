/*
 * A145030  Hexanacci numbers: a(n)=a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6),
 *          {0,1,2,3,4,5...}.
 * https://oeis.org/A145030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6), a(1)=0,
 *            a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=5
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

static seq_t *hexanacci_cache = NULL;   /* zero‑indexed: cache[0] = a(1) */
static size_t   hexanacci_cap   = 0;    /* capacity of hexanacci_cache   */
static idx_t    hexanacci_len   = 0;    /* number of terms currently cached */

seq_t a(idx_t n)
{
    /* if the cache already contains the value, just return it */
    if (n <= hexanacci_len)
        return hexanacci_cache[n - 1];

    /* grow the cache so that index (n-1) is valid */
    ensure_cache(n, &hexanacci_cache, &hexanacci_cap);

    /* compute missing terms up to n */
    while (hexanacci_len < n) {
        idx_t  i   = hexanacci_len + 1;  /* next term to compute (1‑based) */
        seq_t  val;

        if (i <= 6) {
            /* initial values */
            switch (i) {
                case 1: val = 0; break;
                case 2: val = 1; break;
                case 3: val = 2; break;
                case 4: val = 3; break;
                case 5: val = 4; break;
                case 6: val = 5; break;
            }
        } else {
            /* sum the previous six terms */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 6; ++k)
                sum += hexanacci_cache[hexanacci_len - k];
            val = sum;
        }

        hexanacci_cache[hexanacci_len] = val;
        ++hexanacci_len;
    }

    return hexanacci_cache[n - 1];
}
