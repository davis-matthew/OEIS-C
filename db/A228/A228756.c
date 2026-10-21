/*
 * A228756  Number of 4 X n binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or antidiagonally.
 * https://oeis.org/A228756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A228756:
   a(n) = a(n-1) + 20*a(n-2) + 27*a(n-3)
       - 14*a(n-4) - 25*a(n-5) + 4*a(n-6) + 5*a(n-7) - a(n-8)
   Initial values:
   a(1)=8, a(2)=21, a(3)=168, a(4)=780, a(5)=4599,
   a(6)=24246, a(7)=134440, a(8)=728537 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
static size_t cache_cap = 0;     /* capacity allocated (in elements) */
static idx_t  cache_len = 0;     /* largest n that has been computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0; caller is expected to call a(n) for n>=1 */

    /* Allocate or grow cache if necessary */
    if (n > (idx_t)cache_cap) {
        size_t new_cap = (size_t)n;
        if (new_cap < 1) new_cap = 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {          /* allocation failed – abort */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initial values for n = 1 … 8 */
    static const seq_t init[8] = {
        (seq_t)8,
        (seq_t)21,
        (seq_t)168,
        (seq_t)780,
        (seq_t)4599,
        (seq_t)24246,
        (seq_t)134440,
        (seq_t)728537
    };

    /* Fill the initial segment if not yet present */
    if (cache_len < 8) {
        idx_t upto = n < 8 ? n : 8;
        for (idx_t i = cache_len; i < upto; ++i) {
            cache[i] = init[i];
        }
        if (upto > cache_len) cache_len = upto;
    }

    /* Compute remaining terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {   /* i is 1‑based index of desired term */
        /* Coefficients of the recurrence are applied to previous 8 terms */
        seq_t term = cache[i-2];                         /* a(i-1)   */
        term += (seq_t)20 * cache[i-3];                 /* a(i-2)   */
        term += (seq_t)27 * cache[i-4];                 /* a(i-3)   */
        term -= (seq_t)14 * cache[i-5];                 /* a(i-4)   */
        term -= (seq_t)25 * cache[i-6];                 /* a(i-5)   */
        term += (seq_t)4  * cache[i-7];                 /* a(i-6)   */
        term += (seq_t)5  * cache[i-8];                 /* a(i-7)   */
        term -=               cache[i-9];               /* a(i-8)   */
        cache[i-1] = term;
    }

    cache_len = n;
    return cache[n-1];
}
