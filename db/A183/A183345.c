/*
 * A183345  Number of n X 3 binary arrays with each 1 adjacent to exactly one
 *          0 vertically and one 0 horizontally.
 * https://oeis.org/A183345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n)^2 where b(1)=1, b(2)=3, b(3)=4 and for n>=4, b(n) =
 *            b(n-1) + b(n-3) (OEIS A183345)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *b_cache   = NULL;   /* stores values of b(n) */
static size_t   b_size   = 0;      /* number of b values currently stored */
static size_t   b_cap    = 0;      /* allocated capacity for b_cache */

/* ensure capacity for at least 'need' elements in b_cache */
static void grow_b_cache(size_t need)
{
    if (b_cap >= need)
        return;
    size_t new_cap = b_cap ? b_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *newp = (seq_t*)realloc(b_cache, new_cap * sizeof(seq_t));
    if (!newp) abort();                 /* out of memory */
    b_cache = newp;
    b_cap = new_cap;
}

/* initialize the cache with the first three terms of b(n) */
static void init_b_cache(void)
{
    if (b_cache)
        return;
    grow_b_cache(3);
    b_cache[0] = (seq_t)1;   /* b(1) */
    b_cache[1] = (seq_t)3;   /* b(2) */
    b_cache[2] = (seq_t)4;   /* b(3) */
    b_size = 3;
}

/* compute b(n) up to n, if necessary */
static void ensure_b(size_t n)            /* n is 1‑based */
{
    if (n <= b_size)
        return;
    /* compute from current size up to n */
    for (size_t i = b_size; i < n; ++i) {
        /* b(i+1) = b(i) + b(i-2)   (because i+1==n, so i==n-1) */
        seq_t val = b_cache[i - 1] + b_cache[i - 3];
        grow_b_cache(i + 1);          /* ensure space for element i (0‑based i) */
        b_cache[i] = val;
    }
    b_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n=0, return 0 as safe default */
        return (seq_t)0;
    init_b_cache();
    size_t idx = (size_t)n;          /* 1‑based index, fits into size_t */
    ensure_b(idx);
    seq_t val = b_cache[idx - 1];
    return val * val;                /* a(n) = b(n)^2 */
}
