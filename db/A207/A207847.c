/*
 * A207847  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + a(n-3) - a(n-4) (n > 5) Initial
 *            values: a(1) = 10 a(2) = 100 a(3) = 240 a(4) = 780 a(5) = 2320
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of already computed terms */
static idx_t cache_max = 0;     /* maximum n computed so far (number of terms) */

seq_t a(idx_t n)
{
    if (n <= 0) return 0;  /* undefined for n <= 0, but input is always >= 1 */

    /* Extend cache if necessary */
    if (n > cache_max) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;

        /* Compute new terms */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)       val = (seq_t)10;
            else if (i == 2)  val = (seq_t)100;
            else if (i == 3)  val = (seq_t)240;
            else if (i == 4)  val = (seq_t)780;
            else if (i == 5)  val = (seq_t)2320;
            else {
                /* a(i) = a(i-1) + 6*a(i-2) + a(i-3) - a(i-4) */
                seq_t a1 = cache[i-2];          /* a(i-1) */
                seq_t a2 = cache[i-3];          /* a(i-2) */
                seq_t a3 = cache[i-4];          /* a(i-3) */
                seq_t a4 = cache[i-5];          /* a(i-4) */
                val = a1 + (seq_t)6 * a2 + a3 - a4;
            }
            cache[i-1] = val;
        }
        cache_max = n;
    }
    return cache[n-1];
}
