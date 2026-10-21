/*
 * A210119  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock having
 *          two or four distinct values, and new values 0..3 introduced in row
 *          major order.
 * https://oeis.org/A210119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) - 174*a(n-2) + 368*a(n-3) + 303*a(n-4) -
 *            888*a(n-5) - 340*a(n-6) + 240*a(n-7) Initial values: a(1)=44,
 *            a(2)=481, a(3)=5587, a(4)=67585, a(5)=836053, a(6)=10466659,
 *            a(7)=131849767
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init_values[8] = {0, 44, 481, 5587, 67585, 836053, 10466659, 131849767};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;     /* allocated size (n+1) */
    static idx_t max_n = 0;    /* highest n computed so far */

    if (n == 0) return 0; /* safety, though offset is 1 */

    /* Ensure sufficient capacity */
    size_t needed = (n + 1 > 8 ? n + 1 : 8);   /* at least indices 0..7 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * needed);
        if (!cache) return 0; /* allocation failure – caller may handle */
        memcpy(cache, init_values, sizeof(init_values));  /* initialise known values */
        cap = needed;
    } else if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
        if (!tmp) return 0;   /* allocation failure */
        cache = tmp;
        cap = needed;
    }

    /* Compute terms up to n if not already done */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        if (k <= 7) {
            /* already initialised */
            continue;
        }
        seq_t val = cache[k-1] * (seq_t)24;
        val -= cache[k-2] * (seq_t)174;
        val += cache[k-3] * (seq_t)368;
        val += cache[k-4] * (seq_t)303;
        val -= cache[k-5] * (seq_t)888;
        val -= cache[k-6] * (seq_t)340;
        val += cache[k-7] * (seq_t)240;
        cache[k] = val;
    }
    if (n > max_n) max_n = n;

    return cache[n];
}
