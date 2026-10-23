/*
 * A164369  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=7, a(2)=42, a(3)=252, a(4)=1512, a(5)=9072,
 *            a(6)=54432, a(7)=326571. For n≥8: if n==8 : a(n) = 6*a(7) -
 *            20*a(1) + 15*a(0) - 1 else : a(n) = 6*a(n-1) - 20*a(n-7) +
 *            15*a(n-8) (Derived from the generating function
 *            (1+x)*(1‑x⁷)/(1‑6x+20x⁷‑15x⁸).)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache – grows as needed. */
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* allocated capacity */
    static size_t len  = 0;   /* next free position */

    /* Initial 8 terms known directly. */
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)7,     (seq_t)42,   (seq_t)252,
        (seq_t)1512,  (seq_t)9072, (seq_t)54432, (seq_t)326571
    };

    /* Return if already computed. */
    if (n < len && cache)
        return cache[n];

    /* Ensure array large enough. */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = cap == 0 ? 16 : cap * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Fill missing terms up to n. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i < 8) {
            cache[i] = init[i];
        }
        else if (i == 8) {
            /* a(8) = 6*a(7) - 20*a(1) + 15*a(0) - 1. */
            seq_t t = cache[i-1] * (seq_t)6;
            seq_t t7 = cache[i-7] * (seq_t)20;
            seq_t t8 = cache[i-8] * (seq_t)15;
            cache[i] = t - t7 + t8 - (seq_t)1;
        }
        else { /* i >= 9 */
            seq_t t = cache[i-1] * (seq_t)6;
            seq_t t7 = cache[i-7] * (seq_t)20;
            seq_t t8 = cache[i-8] * (seq_t)15;
            cache[i] = t - t7 + t8;
        }
    }
    len = need;
    return cache[(size_t)n];
}
