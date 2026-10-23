/*
 * A044202  Numbers k such that string 1,7 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k such that the octal representation *
 *            of k contains the substring "17" while the octal representation
 *            of k-1 * does not
 * Range:     u8 n <= 5, u16 n <= 1065, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has17_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has17_oct.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored terms a(1), a(2), … */
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static seq_t cur_k = 0;        /* last tested integer k */

    if (n == 0) return 0;          /* offset is 1; undefined for n=0 */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++cur_k;                   /* candidate number */

        if (has17_oct(cur_k)) {
            /* avoid subtraction underflow – cur_k is ≥ 1 here */
            if (!has17_oct(cur_k - 1)) {
                /* store the term */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? 2 * cache_cap : 4;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cur_k;
            }
        }
    }

    return cache[(size_t)(n - 1)];
}
