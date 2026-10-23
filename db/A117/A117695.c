/*
 * A117695  a(n) is the number of pairs of distinct positive integers not
 *          containing a digit 0 giving a sum equal to n.
 * https://oeis.org/A117695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 689, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A117695:
 * a(n) = number of unordered pairs of distinct positive integers
 * that contain no digit 0 and whose sum equals n. */

#include <stdlib.h>
#include <stdbool.h>

/* Test if an integer contains the digit 0. */
static bool has_zero_digit(idx_t x)
{
    while (x > 0) {
        if ((x % 10) == 0)
            return true;
        x /= 10;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* defined only for n >= 1 */

    /* Static storage for results */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* allocated length */
    static idx_t cache_max  = 0;    /* highest n already computed */

    /* Static array to mark numbers without digit 0. */
    static unsigned char *nz = NULL;    /* 1 if number has no 0, 0 otherwise */
    static size_t nz_len = 0;
    static idx_t nz_max  = 0;

    /* Helper to make sure array nz has space for up to N. */
    if (nz_max < n) {
        size_t new_len = (nz_len == 0) ? 1 : nz_len;
        while (new_len <= (size_t)n)
            new_len <<= 1;

        unsigned char *tmp = (unsigned char *)realloc(nz, new_len);
        if (!tmp)
            exit(1);   /* allocation failure */
        nz = tmp;
        nz_len = new_len;

        /* Compute values for new indices. */
        for (idx_t i = nz_max + 1; i <= n; ++i)
            nz[i] = has_zero_digit(i) ? 0 : 1;
        nz_max = n;
    }

    /* Helper to make sure cache array can hold n. */
    if (cache_len <= (size_t)n) {
        size_t new_len = (cache_len == 0) ? 1 : cache_len;
        while (new_len <= (size_t)n)
            new_len <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute values up to n if needed. */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t cnt = 0;
        for (idx_t a = 1; a * 2 < m; ++a) {   /* ensure a < b */
            idx_t b = m - a;
            if (nz[a] && nz[b])
                ++cnt;
        }
        cache[m] = cnt;
    }
    cache_max = n;

    return cache[n];
}
