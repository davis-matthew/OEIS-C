/*
 * A191830  Expansion of x^2*(2-3*x)/(1-x-x^2)^2.
 * https://oeis.org/A191830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-2*a(n-3)-a(n-4); a(0)=0, a(1)=0, a(2)=2,
 *            a(3)=1, a(4)=4 (valid for all n ≥ 0, using zero‑based indices)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* ensure capacity for indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 4;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort gracefully */
            return 0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val;
        switch (i) {
            case 0: val = 0; break;
            case 1: val = 0; break;
            case 2: val = 2; break;
            case 3: val = 1; break;
            case 4: val = 4; break;
            default:
                val = (2 * cache[i - 1]) + cache[i - 2]
                    - (2 * cache[i - 3]) - cache[i - 4];
                break;
        }
        cache[i] = val;
    }
    len = (size_t)n + 1;
    return cache[(size_t)n];
}
