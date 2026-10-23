/*
 * A039103  Numbers whose base-9 representation has the same number of 4's and
 *          6's.
 * https://oeis.org/A039103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A039103: numbers whose base‑9 representation
 * has the same number of 4's and 6's.  The sequence starts with a(1)=0,
 * a(2)=1, a(3)=2, a(4)=3, a(5)=5, … */
#include <stdlib.h>
#include <stdbool.h>

/* Cache of already found terms.  cache[0] = 0 for a(1). */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t  cache_len = 0;         /* number of terms stored in cache */
static idx_t  last_tested = 0;       /* last integer inspected for validity */

/* Test whether an integer has the same number of 4's and 6's in base 9. */
static bool valid_base9(idx_t num)
{
    int cnt4 = 0, cnt6 = 0;
    if (num == 0) return true;                /* 0 has no 4's or 6's */

    idx_t x = num;
    while (x > 0) {
        int d = (int)(x % 9);
        if (d == 4) cnt4++;
        else if (d == 6) cnt6++;
        x /= 9;
    }
    return cnt4 == cnt6;
}

/* Grow the cache array to at least new_cap elements.  */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < cache_len) new_cap = cache_len;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* Out of memory: terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined by definition, but guard anyway */

    /* Initialise cache with a(1)=0 on the first call. */
    if (cache_len == 0) {
        if (cache_cap == 0) grow_cache();
        cache[0] = (seq_t)0;
        cache_len = 1;
        last_tested = 0;
    }

    if (n <= cache_len)
        return cache[n - 1];

    if (cache_len >= cache_cap) grow_cache();

    /* Generate terms until we reach the requested index. */
    while (cache_len < n) {
        /* Increment last_tested until a valid number is found. */
        do {
            ++last_tested;
        } while (!valid_base9(last_tested));

        cache[cache_len++] = (seq_t)last_tested;
        if (cache_len >= cache_cap) grow_cache();
    }

    return cache[n - 1];
}
