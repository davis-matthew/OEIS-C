/*
 * A235948  Number of (n+1) X (2+1) 0..2 arrays colored with the upper median
 *          value of each 2 X 2 subblock.
 * https://oeis.org/A235948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-3) + 10*a(n-4) - 12*a(n-7) - 6*a(n-8), n>12 Initial
 *            values: a(1)=294, a(2)=722, a(3)=1372, a(4)=2660, a(5)=5034,
 *            a(6)=9752, a(7)=18468, a(8)=35794, a(9)=67924, a(10)=131474,
 *            a(11)=251786, a(12)=485354
 * Range:     u8 none, u16 n <= 8, u32 n <= 25, u64 n <= 59, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[12] = {
    294ULL, 722ULL, 1372ULL, 2660ULL, 5034ULL, 9752ULL,
    18468ULL, 35794ULL, 67924ULL, 131474ULL, 251786ULL, 485354ULL
};

static seq_t *cache = NULL;      /* array of computed terms, 0‑indexed */
static idx_t cached = 0;         /* number of terms currently cached */

/* extend the cache to hold at least n terms (n >= 1) */
static void extend_cache(idx_t n)
{
    if (cached >= n) return;
    idx_t new_size = n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) { /* allocation failed – abort */
        abort();
    }
    cache = new_cache;

    /* fill new entries */
    for (idx_t i = cached + 1; i <= n; ++i) {
        if (i <= 12) {
            cache[i - 1] = init_vals[i - 1];
        } else {
            /* recurrence uses indices i-3,i-4,i-7,i-8 which are all < i */
            seq_t v = 3 * cache[i - 3 - 1] + 10 * cache[i - 4 - 1]
                      - 12 * cache[i - 7 - 1] - 6 * cache[i - 8 - 1];
            cache[i - 1] = v;
        }
    }
    cached = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behavior below offset is irrelevant */
    extend_cache(n);
    return cache[n - 1];
}
