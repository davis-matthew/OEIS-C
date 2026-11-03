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
 * Validated: standard, pass, 64 terms through n = 8709 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: does the octal representation of x contain the substring "17"? */
static bool has17_oct(seq_t x)
{
    if (x == 0) return false;
    seq_t t = x;
    unsigned int prev = 255;               /* sentinel value, not a digit */

    while (t != 0) {
        unsigned int d = (unsigned int)(t % 8);
        /* Detect pattern most‑significant 1 followed by least‑significant 7
         * by reading digits from least significant to most significant.
         * Thus we look for a digit 1 after seeing a digit 7. */
        if (d == 1 && prev == 7) return true;
        prev = d;
        t = t / 8;
    }
    return false;
}

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
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
