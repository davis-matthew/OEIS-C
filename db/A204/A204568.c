/*
 * A204568  Number of (n+1)X5 0..3 arrays with every 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A204568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1682, a(2)=80518, … (first 13 terms given) Empirical linear
 *            recurrence (n>=14):
 *            a(n)=66*a(n-1)-371*a(n-2)-20540*a(n-3)+324789*a(n-4)
 *            -1657430*a(n-5)+2548727*a(n-6)+4497416*a(n-7)
 *            -19534224*a(n-8)+23561568*a(n-9)-10191744*a(n-10)
 *            -1130112*a(n-11)+1975104*a(n-12)-373248*a(n-13) For n<1 return 0
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   mul_small, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc, size_t */
#include <stdbool.h>         /* bool */
#include "../utils/mul_small.h"   /* mul_small(seq_t, unsigned int) */
#include "../utils/seq_from_str.h"/* seq_from_str(const char*) */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;   /* allocated capacity (number of slots) */
    static size_t  len    = 0;   /* number of values stored in cache */

    if (n == 0)                  /* a(0) undefined, return 0 */
        return (seq_t)0;

    size_t idx = (size_t)n;      /* n fits in size_t as n>=1 */

    /* Return cached value if available */
    if (idx <= len)
        return cache[idx - 1];

    /* Ensure allocated capacity is enough */
    if (cap == 0)
        cap = 16;
    while (cap < idx)
        cap <<= 1;
    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));

    /* Initial known terms (first 13) parsed once */
    static const char *init_str[13] = {
        "1682",
        "80518",
        "4220392",
        "226207388",
        "12184154082",
        "657035534014",
        "35439471578008",
        "1911668983879844",
        "103120042408983914",
        "5562564379359047286",
        "300059383373323671216",
        "16185997942290302282060",
        "873115606316546208696882"
    };
    static seq_t init_val[13];
    static bool init_done = false;
    if (!init_done) {
        for (int i = 0; i < 13; ++i)
            init_val[i] = seq_from_str(init_str[i]);
        init_done = true;
    }

    /* Compute terms up to requested index */
    for (size_t i = len; i < idx; ++i) {
        if (i < 13) {                /* initial segment */
            cache[i] = init_val[i];
        } else {
            /* recurrence uses 13 previous terms */
            seq_t res = 0;
            res += mul_small(cache[i - 1], (unsigned int)66);
            res -= mul_small(cache[i - 2], (unsigned int)371);
            res -= mul_small(cache[i - 3], (unsigned int)20540);
            res += mul_small(cache[i - 4], (unsigned int)324789);
            res -= mul_small(cache[i - 5], (unsigned int)1657430);
            res += mul_small(cache[i - 6], (unsigned int)2548727);
            res += mul_small(cache[i - 7], (unsigned int)4497416);
            res -= mul_small(cache[i - 8], (unsigned int)19534224);
            res += mul_small(cache[i - 9], (unsigned int)23561568);
            res -= mul_small(cache[i -10], (unsigned int)10191744);
            res -= mul_small(cache[i -11], (unsigned int)1130112);
            res += mul_small(cache[i -12], (unsigned int)1975104);
            res -= mul_small(cache[i -13], (unsigned int)373248);

            cache[i] = res;
        }
    }

    len = idx;                     /* update cached length */
    return cache[idx - 1];
}
