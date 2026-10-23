/*
 * A166535  Positive integers whose binary representation does not contain a
 *          run of more than three consecutive 0's or 1's.
 * https://oeis.org/A166535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer whose binary representation * does
 *            not contain a run of more than three consecutive 0's or 1's
 * Range:     u8 n <= 176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cache of previously computed terms. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

/* Return true if x satisfies the run‑length constraint. */
static bool passes(seq_t x)
{
    if (x == 0)
        return true;          /* 0 is not used, but harmless */

    seq_t y = x;
    int prev_bit = -1;        /* bit seen last (0 or 1) */
    int run      = 0;         /* current run length */

    while (y > 0)             /* iterate over bits from LSB to MSB */
    {
        int bit = (int)(y & 1);    /* least‑significant bit */
        if (bit == prev_bit)
            ++run;
        else
        {
            run = 1;
            prev_bit = bit;
        }

        if (run > 3)
            return false;       /* run too long */

        y = y / 2;              /* shift right by one bit */
    }

    return true;
}

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;

    idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* allocation failure */

    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of A166535 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined, but safe fallback */

    /* Return cached value if already computed. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow cache to accommodate the required index. */
    ensure_capacity(n);

    /* Start searching from the next integer after the last cached value. */
    seq_t val = (cache_size == 0) ? 1 : cache[cache_size - 1] + 1;

    /* Find and cache terms until the n-th one is reached. */
    while (cache_size < n)
    {
        if (passes(val))
            cache[cache_size++] = val;
        ++val;
    }

    return cache[n - 1];
}
