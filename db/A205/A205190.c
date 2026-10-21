/*
 * A205190  Number of (n+1) X 6 0..1 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock differing from each horizontal
 *          or vertical neighbor.
 * https://oeis.org/A205190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)=144, a(2)=168, a(3)=292, a(4)=384, a(5)=736,
 *            a(6)=896, a(7)=1568, a(8)=1920, a(9)=3392; for n>9:
 *            a(n)=2*a(n-2)+a(n-6). The function memoizes the values so
 *            repeated calls are O(1)
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 44, u64 n <= 100, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t size = 0;          /* number of values currently stored */
static size_t cap = 0;          /* allocated capacity */

/* Initial values for n = 1 .. 9 */
static const seq_t init[9] = {
    (seq_t)144, (seq_t)168, (seq_t)292, (seq_t)384, (seq_t)736,
    (seq_t)896, (seq_t)1568, (seq_t)1920, (seq_t)3392
};

/* Ensure that 'cache' has at least 'needed' elements allocated. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cap)
        return;
    size_t new_cap = needed * 2;
    if (new_cap < 16)
        new_cap = 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Out of memory; terminate program. */
        exit(1);
    }
    cache = tmp;
    cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined; return 0 for safety */

    ensure_capacity(n);

    /* Compute values up to index n. */
    for (idx_t i = size; i < n; ++i) {
        if (i < 9) {
            cache[i] = init[i];
        } else {
            /* a(n) = 2*a(n-2) + a(n-6), zero‑based indices: */
            cache[i] = 2 * cache[i - 2] + cache[i - 6];
        }
    }
    size = n;

    return cache[n - 1]; /* zero‑based offset */
}
