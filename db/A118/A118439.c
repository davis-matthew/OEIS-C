/*
 * A118439  Column 0 of triangle A118438.
 * https://oeis.org/A118439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies the linear recurrence a(n) = -5*a(n-2) -
 *            19*a(n-4) + 25*a(n-6) for n ≥ 6, with initial values a(0) = 1,
 *            a(1) = -1, a(2) = 5, a(3) = 11, a(4) = -23, a(5) = -41. This
 *            follows from the generating function: (1 - x + 10x² + 6x³ + 21x⁴
 *            - 5x⁵) / (1 - x²)(1 + 6x² + 25x⁴)
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 27, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* cache: array holding computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;   /* allocated size          */
    static idx_t  computed= 0;   /* number of terms actually
                                 * known (0 .. computed-1) */

    /* Ensure we have space for all indices up to n. */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 6) new_cap = 6;               /* at least 6 for safety   */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                         /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms (if any) starting from the first
       uncached index. The loop is safe because recurrence only
       refers back to indices that are already known. */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t val;
        switch (i) {
            case 0:  val = (seq_t)  1; break;
            case 1:  val = (seq_t) -1; break;
            case 2:  val = (seq_t)  5; break;
            case 3:  val = (seq_t) 11; break;
            case 4:  val = (seq_t)-23; break;
            case 5:  val = (seq_t)-41; break;
            default:
                /* Recurrence for n >= 6 */
                val = - (seq_t)5  * cache[i - 2]
                      - (seq_t)19 * cache[i - 4]
                      + (seq_t)25 * cache[i - 6];
                break;
        }
        cache[i] = val;
    }
    computed = (n + 1 > computed) ? (n + 1) : computed;

    return cache[n];
}
