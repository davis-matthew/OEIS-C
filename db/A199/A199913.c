/*
 * A199913  Number of -n..n arrays x(0..5) of 6 elements with zero sum, and
 *          adjacent elements not equal modulo three (with -1 modulo 3 = 2).
 * https://oeis.org/A199913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 4*a(n-3) - 8*a(n-4) + 4*a(n-5) -
 *            6*a(n-6) +12*a(n-7) - 6*a(n-8) + 4*a(n-9) - 8*a(n-10) +
 *            4*a(n-11) - a(n-12) + 2*a(n-13) - a(n-14) a(1) = 32, a(2) = 232,
 *            a(3) = 1312, a(4) = 5016, a(5) = 12872, a(6) = 29864, a(7) =
 *            62776, a(8) = 114768, a(9) = 200520, a(10) = 335216, a(...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 69, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[15] = {0,
    32, 232, 1312, 5016, 12872, 29864, 62776, 114768,
    200520, 335216, 522160, 792880, 1174320, 1666712};

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t computed_up_to = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* Undefined for n<1 */
    size_t idx = (size_t)n;

    /* Grow cache if necessary */
    if (cache_size < idx + 1) {
        size_t new_size = cache_size ? cache_size * 2 : 64;
        if (new_size < idx + 1) new_size = idx + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);             /* allocation failure */
        /* initialise new slots to 0 */
        for (size_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise first 14 values once */
    if (computed_up_to == 0) {
        for (size_t i = 1; i <= 14; ++i) cache[i] = init_vals[i];
        computed_up_to = 14;
    }

    /* Return cached value if already computed */
    if (idx <= computed_up_to) return cache[idx];

    /* Compute missing values up to idx using the recurrence */
    for (size_t k = computed_up_to + 1; k <= idx; ++k) {
        seq_t value = 0;
        value += cache[k - 1] * 2;
        value -= cache[k - 2];
        value += cache[k - 3] * 4;
        value -= cache[k - 4] * 8;
        value += cache[k - 5] * 4;
        value -= cache[k - 6] * 6;
        value += cache[k - 7] * 12;
        value -= cache[k - 8] * 6;
        value += cache[k - 9] * 4;
        value -= cache[k - 10] * 8;
        value += cache[k - 11] * 4;
        value -= cache[k - 12];
        value += cache[k - 13] * 2;
        value -= cache[k - 14];
        cache[k] = value;
    }
    computed_up_to = idx;
    return cache[idx];
}
