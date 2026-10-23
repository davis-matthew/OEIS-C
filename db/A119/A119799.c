/*
 * A119799  Numbers m such that m, m+1 and 2*m have the same number of
 *          distinct digits in decimal representation.
 * https://oeis.org/A119799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 9192, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A119799: Numbers m such that m, m+1 and 2*m have the same number of distinct decimal digits.
   The function generates terms lazily and caches them. */

#include <stddef.h>
#include <stdlib.h>

/* Count distinct decimal digits of x in its decimal representation.  */
static int distinct_digits(seq_t x)
{
    unsigned char seen[10] = {0};
    if (x == 0)
        seen[0] = 1;
    for (seq_t tmp = x; tmp > 0; tmp /= 10) {
        int d = (int)(tmp % 10);
        seen[d] = 1;
    }
    int count = 0;
    for (int i = 0; i < 10; ++i)
        count += seen[i];
    return count;
}

/* Return the nth term (1‑based) of A119799.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* dynamic array of found terms           */
    static size_t cache_len = 0;    /* number of terms currently stored       */
    static size_t cache_cap = 0;    /* current capacity of the array           */
    static seq_t next_m = 0;        /* next candidate to test                  */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t m = next_m;
        int d1 = distinct_digits(m);
        int d2 = distinct_digits(m + 1);
        int d3 = distinct_digits(m + m);

        if (d1 == d2 && d2 == d3) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
#ifdef __cplusplus
                seq_t *new_ptr = new seq_t[new_cap];
                if (cache) {
                    for (size_t i = 0; i < cache_len; ++i)
                        new_ptr[i] = cache[i];
                }
                delete[] cache;
                cache = new_ptr;
#else
                seq_t *new_ptr = realloc(cache, new_cap * sizeof(seq_t));
                if (!new_ptr) abort();   /* allocation failure */
                cache = new_ptr;
#endif
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
        ++next_m;
    }
    return cache[n - 1];
}
