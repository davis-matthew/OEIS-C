/*
 * A229638  Number of defective 3-colorings of a 2 X n 0..2 array connected
 *          horizontally, diagonally and antidiagonally with exactly two
 *          mistakes, and colors introduced in row-major 0..2 order.
 * https://oeis.org/A229638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) - 16*a(n-3) + 12*a(n-4) + 24*a(n-5) +
 *            8*a(n-6) with initial values: a(1)=0, a(2)=6, a(3)=40, a(4)=122,
 *            a(5)=488, a(6)=1608
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values, 0‑based index */
static size_t cap   = 0;          /* allocated capacity */
static size_t used  = 0;          /* number of values stored */

/* enlarge the cache to hold at least n elements */
static void ensure_capacity(size_t n)
{
    if (cap < n) {
        size_t new_cap = cap == 0 ? 8 : cap * 2;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* In practice realloc will never fail for the limits we use. */
        cache = tmp;
        cap = new_cap;
    }
}

/* initialise the first six terms */
static void init_first_terms(void)
{
    if (used >= 6) return;
    ensure_capacity(6);
    cache[0] = (seq_t)0;    /* a(1) */
    cache[1] = (seq_t)6;    /* a(2) */
    cache[2] = (seq_t)40;   /* a(3) */
    cache[3] = (seq_t)122;  /* a(4) */
    cache[4] = (seq_t)488;  /* a(5) */
    cache[5] = (seq_t)1608; /* a(6) */
    used = 6;
}

/* compute a(n) for any n >= 1 */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined, but guard */
    size_t idx = (size_t)n - 1;          /* zero based index */

    /* initialise and grow cache as needed */
    init_first_terms();
    if (idx < used) return cache[idx];

    ensure_capacity(idx + 1);

    /* compute missing terms up to idx using recurrence */
    for (size_t i = used; i <= idx; ++i) {
        /* recurrence: a(i+1) depends on previous six terms */
        seq_t t1 = cache[i - 1];   /* a(i)   */
        seq_t t2 = cache[i - 2];   /* a(i-1) */
        seq_t t3 = cache[i - 3];   /* a(i-2) */
        seq_t t4 = cache[i - 4];   /* a(i-3) */
        seq_t t5 = cache[i - 5];   /* a(i-4) */
        seq_t t6 = cache[i - 6];   /* a(i-5) */

        seq_t val = t1 * (seq_t)6
                    - t2 * (seq_t)6
                    - t3 * (seq_t)16
                    + t4 * (seq_t)12
                    + t5 * (seq_t)24
                    + t6 * (seq_t)8;
        cache[i] = val;
    }
    used = idx + 1;
    return cache[idx];
}
