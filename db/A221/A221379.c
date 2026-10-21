/*
 * A221379  Number of nX5 arrays of occupancy after each element moves to some
 *          horizontal or antidiagonal neighbor, with every occupancy equal to
 *          zero or two.
 * https://oeis.org/A221379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for odd n; for even n the sequence satisfies a(n) =
 *            42*a(n-2) - 497*a(n-4) + 2546*a(n-6) - 6335*a(n-8) +
 *            7910*a(n-10) - 5003*a(n-12) + 1526*a(n-14) - 196*a(n-16) +
 *            8*a(n-18). The base values for even n up to 18 are taken from
 *            the OEIS data: a(2)=8, a(4)=189, a(6)=5040, a(8)=134521,
 *            a(10)...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* maximum n for which we provide a static cache */
    enum { MAX_CACHE_N = 10000 };

    /* static array to cache computed values */
    static seq_t cache[MAX_CACHE_N + 1];
    static bool initialized = false;
    static idx_t computed_up_to = 0;

    /* initialise base values once */
    if (!initialized) {
        /* zero all entries first */
        for (idx_t i = 0; i <= MAX_CACHE_N; ++i)
            cache[i] = 0;
        /* base values for even n */
        cache[2]  = (seq_t)8;
        cache[4]  = (seq_t)189;
        cache[6]  = (seq_t)5040;
        cache[8]  = (seq_t)134521;
        cache[10] = (seq_t)3580920;
        cache[12] = (seq_t)95235525;
        cache[14] = (seq_t)2532193248;
        cache[16] = (seq_t)67323825121;
        cache[18] = (seq_t)1789924262120;
        computed_up_to = 18;
        initialized = true;
    }

    if (n < 1)
        return 0;          /* undefined for n < 1, return 0 */

    /* request larger n than cached -> extend computation */
    if (n > computed_up_to) {
        idx_t limit = n;
        if (limit > MAX_CACHE_N)
            limit = MAX_CACHE_N;  /* clamp to array size */

        for (idx_t i = computed_up_to + 1; i <= limit; ++i)
        {
            if ((i & 1) != 0) {
                cache[i] = 0;       /* odd indices */
            } else if (i <= 18) {
                /* already seeded */ ;
            } else {
                /* recurrence for even n >= 20 */
                seq_t val = ((seq_t)42) * cache[i-2];
                val -= ((seq_t)497) * cache[i-4];
                val += ((seq_t)2546) * cache[i-6];
                val -= ((seq_t)6335) * cache[i-8];
                val += ((seq_t)7910) * cache[i-10];
                val -= ((seq_t)5003) * cache[i-12];
                val += ((seq_t)1526) * cache[i-14];
                val -= ((seq_t)196) * cache[i-16];
                val += ((seq_t)8) * cache[i-18];
                cache[i] = val;
            }
        }
        computed_up_to = limit;
    }

    if (n <= computed_up_to)
        return cache[n];

    /* n larger than CACHE_SIZE: compute on the fly without caching */
    if ((n & 1) != 0)
        return 0;          /* odd */

    /* compute value for large even n using recurrence only on needed entries */
    /* store last 9 even terms in a ring buffer */
    seq_t ring[9] = {0};
    /* fill ring with known small terms up to 18 */
    idx_t start = 2;
    idx_t end = 18;
    idx_t idx = 0;
    for (idx_t i = start; i <= end; i += 2) {
        ring[idx] = cache[i];
        idx = (idx + 1) % 9;
    }
    idx_t cur = idx;   /* index of next to overwrite */

    /* iterate from 20 to n in steps of 2 */
    for (idx_t i = 20; i <= n; i += 2)
    {
        seq_t val = ((seq_t)42)  * ring[(cur + 8) % 9];
        val -= ((seq_t)497) * ring[(cur + 7) % 9];
        val += ((seq_t)2546) * ring[(cur + 6) % 9];
        val -= ((seq_t)6335) * ring[(cur + 5) % 9];
        val += ((seq_t)7910) * ring[(cur + 4) % 9];
        val -= ((seq_t)5003) * ring[(cur + 3) % 9];
        val += ((seq_t)1526) * ring[(cur + 2) % 9];
        val -= ((seq_t)196)  * ring[(cur + 1) % 9];
        val += ((seq_t)8)    * ring[cur];
        ring[cur] = val;
        cur = (cur + 1) % 9;
    }

    /* find the position of the last computed term in ring */
    idx_t pos = (cur + 8) % 9;   /* last term corresponds to n */
    return ring[pos];
}
