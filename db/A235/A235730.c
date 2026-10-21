/*
 * A235730  Number of (n+1)X(2+1) 0..1 arrays with the sum of each 2X2
 *          subblock maximum and minimum lexicographically nondecreasing
 *          columnwise and nonincreasing rowwise.
 * https://oeis.org/A235730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 11*a(n-2) - 56*a(n-3) + 133*a(n-4) + 13*a(n-5)
 *            - 220*a(n-6) + 73*a(n-7) + 147*a(n-8) - 64*a(n-9) - 45*a(n-10) +
 *            19*a(n-11) + 5*a(n-12) - 2*a(n-13) Initial values: a(1)=58,
 *            a(2)=380, a(3)=2481, a(4)=15963, a(5)=102683, a(6)=659235,
 *            a(7)=4232565, a(8)=27168327, a(9)=174393813, a(1...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms (1‑based indexing) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* size of allocated array */

    /* static array of the 13 initial values */
    static seq_t init[13];
    static bool init_inited = false;
    if (!init_inited) {
        const char *init_str[13] = {
            "58","380","2481","15963","102683",
            "659235","4232565","27168327","174393813",
            "1119403274","7185285384","46121125726","296043902819"
        };
        for (int i = 0; i < 13; ++i)
            init[i] = seq_from_str(init_str[i]);
        init_inited = true;
    }

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* memory exhausted */
        /* zero out the new entries (optional) */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)(cache_cap > 0 ? 1 : 1); i <= n; ++i) {
        if (i <= 13) {
            cache[i] = init[i - 1];
        } else {
            seq_t r = 9 * cache[i - 1];
            r -= 11 * cache[i - 2];
            r -= 56 * cache[i - 3];
            r += 133 * cache[i - 4];
            r += 13 * cache[i - 5];
            r -= 220 * cache[i - 6];
            r += 73 * cache[i - 7];
            r += 147 * cache[i - 8];
            r -= 64 * cache[i - 9];
            r -= 45 * cache[i - 10];
            r += 19 * cache[i - 11];
            r += 5 * cache[i - 12];
            r -= 2 * cache[i - 13];
            cache[i] = r;
        }
    }

    return cache[n];
}
