/*
 * A228972  Number of n X 2 0..2 arrays of the median of the corresponding
 *          element, the element to the east and the element to the south in a
 *          larger (n+1) X 3 0..2 array.
 * https://oeis.org/A228972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence from OEIS entry A228972
   a(n) = 13*a(n-1) -26*a(n-2) -122*a(n-3) +179*a(n-4) +248*a(n-5)
          -404*a(n-6) +135*a(n-7) +130*a(n-8) -374*a(n-9) +297*a(n-10)
          -120*a(n-11) +45*a(n-12)
   initial values: a(1)=9, a(2)=81, a(3)=729, a(4)=6387, a(5)=55887,
   a(6)=488037, a(7)=4259277, a(8)=37155951, a(9)=324050745,
   a(10)=2825717151, a(11)=24637772289, a(12)=214806377331
*/
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache[0] is unused; cache[i] holds a(i) for i>=1 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of allocated slots (including unused 0) */

    /* Ensure cache is large enough for n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;        /* +1 to keep index 0 unused */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;      /* allocation failure – return 0 */
        cache = new_cache;

        /* Initialize new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) continue;      /* skip unused slot */

            if (i <= 12) {              /* seed values */
                switch (i) {
                    case 1:  cache[i] = (seq_t)9ULL;                  break;
                    case 2:  cache[i] = (seq_t)81ULL;                 break;
                    case 3:  cache[i] = (seq_t)729ULL;                break;
                    case 4:  cache[i] = (seq_t)6387ULL;               break;
                    case 5:  cache[i] = (seq_t)55887ULL;              break;
                    case 6:  cache[i] = (seq_t)488037ULL;             break;
                    case 7:  cache[i] = (seq_t)4259277ULL;            break;
                    case 8:  cache[i] = (seq_t)37155951ULL;           break;
                    case 9:  cache[i] = (seq_t)324050745ULL;          break;
                    case 10: cache[i] = (seq_t)2825717151ULL;         break;
                    case 11: cache[i] = (seq_t)24637772289ULL;        break;
                    case 12: cache[i] = (seq_t)214806377331ULL;       break;
                }
            } else {                     /* recurrence for i>12 */
                seq_t pos = 0, neg = 0;
                pos += cache[i-1] * 13ULL;
                neg += cache[i-2] * 26ULL;
                neg += cache[i-3] * 122ULL;
                pos += cache[i-4] * 179ULL;
                pos += cache[i-5] * 248ULL;
                neg += cache[i-6] * 404ULL;
                pos += cache[i-7] * 135ULL;
                pos += cache[i-8] * 130ULL;
                neg += cache[i-9] * 374ULL;
                pos += cache[i-10] * 297ULL;
                neg += cache[i-11] * 120ULL;
                pos += cache[i-12] * 45ULL;
                /* compute result: pos - neg */
                cache[i] = pos - neg;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
