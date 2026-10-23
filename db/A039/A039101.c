/*
 * A039101  Numbers whose base-9 representation has the same number of 3's and
 *          8's.
 * https://oeis.org/A039101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth non‑negative integer whose base‑9 representation
 *            contains an equal number of 3's and 8's
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <new>
#endif

/* Count the number of 3's and 8's in the base‑9 representation of x.
   Returns true iff the counts are equal. */
static bool qualifies(idx_t x)
{
    unsigned c3 = 0, c8 = 0;
    if (x == 0)           /* 0 has zero 3's and zero 8's – qualifies */
        return true;
    while (x > 0) {
        unsigned d = x % 9;
        if (d == 3) ++c3;
        else if (d == 8) ++c8;
        x /= 9;
    }
    return c3 == c8;
}

/* Dynamic cache of previously computed terms.
   cache[i] stores the (i+1)-th term. */
static seq_t *cache       = NULL;  /* 0‑based array of seq_t */
static size_t cap         = 0;     /* allocated capacity */
static idx_t  count       = 0;     /* number of terms stored */
static idx_t  last_num    = 0;     /* numeric value of the last found term */

/* Grow the cache to at least new_cap entries. */
static void grow_cache(size_t new_cap)
{
    size_t old_cap = cap;
    cap = new_cap;
    /* Use realloc; cast its result appropriately. */
    seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
#ifdef __cplusplus
    /* Construct any new seq_t objects that were not previously initialized. */
    for (size_t i = old_cap; i < cap; ++i)
        new(&cache[i]) seq_t();
#endif
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1; return 0 as a safe default */
        return 0;

    /* Compute terms until we have at least n of them. */
    while (count < n) {
        /* Start searching from the next integer after the last found number. */
        idx_t candidate = (count == 0) ? 0 : (last_num + 1);

        /* Find the next qualifying number. */
        while (!qualifies(candidate))
            ++candidate;

        /* Ensure the cache has space for the new entry. */
        if (count >= cap)
            grow_cache((cap == 0) ? 16 : cap * 2);

        /* Store the term. */
        cache[count] = (seq_t)candidate;
        ++count;
        last_num = candidate;
    }

    return cache[n - 1];
}
