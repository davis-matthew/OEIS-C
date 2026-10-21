/*
 * A232336  Number of 2 X n 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or antidiagonally, with no adjacent
 *          elements equal.
 * https://oeis.org/A232336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3) + a(n-4) - a(n-5); a(1)=1,
 *            a(2)=6, a(3)=18, a(4)=32, a(5)=82, a(6)=162
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;
static idx_t  capacity   = 0;
static idx_t  computed   = 0;

static void init_cache(void) {
    if (cache == NULL) {
        capacity = 8;                     /* at least the first 6 terms */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)18;
        cache[3] = (seq_t)32;
        cache[4] = (seq_t)82;
        cache[5] = (seq_t)162;

        computed = 6;
    }
}

seq_t a(idx_t n) {
    init_cache();
    if (n <= computed)          /* already computed */
        return cache[n - 1];

    /* grow the cache if needed */
    if (n > capacity) {
        idx_t newcap = capacity;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        capacity = newcap;
    }

    /* fill missing terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* i is the sequence index (1‑based).
           array index is i‑1. */
        seq_t mult = cache[i - 3];     /* a(i-2) */
        mult += cache[i - 3];          /* +a(i-2) */
        mult += cache[i - 3];          /* +a(i-2) => 3*a(i-2) */

        seq_t val = cache[i - 2];      /* a(i-1) */
        val += mult;                   /* +3*a(i-2) */
        val -= cache[i - 4];           /* -a(i-3) */
        val += cache[i - 5];           /* +a(i-4) */
        val -= cache[i - 6];           /* -a(i-5) */

        cache[i - 1] = val;
    }
    computed = n;
    return cache[n - 1];
}
