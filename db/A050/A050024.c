/*
 * A050024  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 3 - 2^(p+1) and p
 *          is the smallest integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A050024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(m) for n ≥ 4 where m = 2·n – 3 – 2^(p+1), p =
 *            min { k : 2^k < n‑1 ≤ 2^(k+1) } a(1) = a(2) = a(3) = 1 (This is
 *            the recurrence for OEIS A050024.)
 * Range:     u8 n <= 25, u16 n <= 59, u32 n <= 197, u64 n <= 771,
 *            u128 n <= 3649, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache for computed values, 1‑based index */
static seq_t *cache   = NULL;
static unsigned char *calculated = NULL;
static idx_t cache_size = 0;    /* current size allocated (0 … cache_size-1) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* extend the cache if necessary */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;   /* indices 0 … n inclusive */

        seq_t  *new_cache  = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        unsigned char *new_calc = (unsigned char *)realloc(calculated, new_size);

        if (!new_cache || !new_calc) {   /* allocation failed */
            return 0;
        }

        /* zero the new part of the calculated flag array */
        if (new_size > old_size)
            memset(new_calc + old_size, 0, (new_size - old_size));

        cache      = new_cache;
        calculated = new_calc;
        cache_size = new_size;

        /* compute all newly available entries */
        for (idx_t i = old_size; i <= n; ++i) {
            if (i <= 3) {
                cache[i] = (seq_t)1;
                calculated[i] = 1;
            } else {
                /* determine 2^(p+1) – the smallest power of two > (i-2) */
                idx_t x = i - 2;          /* x ≥ 2 for i ≥ 4 */
                idx_t pow2 = 1;
                while (pow2 <= x) pow2 <<= 1;   /* pow2 == 2^(p+1) */

                idx_t m = 2 * i - 3 - pow2;     /* guaranteed 1 ≤ m ≤ i-2 */

                cache[i] = cache[i - 1] + cache[m];
                calculated[i] = 1;
            }
        }
    }
    return cache[n];
}
