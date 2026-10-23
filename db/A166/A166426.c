/*
 * A166426  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*Sum_{j=1..10} a(n-j) - 465*a(n-11) (n ≥ 11) Initial
 *            values: a(0)=1, a(1)=32, a(2)=992, a(3)=30752, a(4)=953312,
 *            a(5)=29552672, a(6)=916132832, a(7)=28400117792,
 *            a(8)=880403651552, a(9)=27292513198112, a(10)=846067909141472,
 *            a(11)=26228105183385136
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;   /* pointer to allocated array    */
    static idx_t  used  = 0;      /* number of computed values (next free index) */
    static idx_t  cap   = 0;      /* allocated capacity              */

    /* 12 base terms (a(0) .. a(11)) */
    static const seq_t init[12] = {
        (seq_t)1ULL,
        (seq_t)32ULL,
        (seq_t)992ULL,
        (seq_t)30752ULL,
        (seq_t)953312ULL,
        (seq_t)29552672ULL,
        (seq_t)916132832ULL,
        (seq_t)28400117792ULL,
        (seq_t)880403651552ULL,
        (seq_t)27292513198112ULL,
        (seq_t)846067909141472ULL,
        (seq_t)26228105183385136ULL
    };

    /* already computed? */
    if (n < used)
        return cache[n];

    /* allocate or grow cache as needed */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t*)malloc((size_t)new_cap * sizeof(seq_t));
        if (!tmp)                /* allocation failure – shouldn't happen */
            return (seq_t)0;
        if (cache) {
            memcpy(tmp, cache, (size_t)used * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = used; i <= n; ++i) {
        if (i < 12) {
            cache[i] = init[i];
        } else {
            seq_t sum10 = 0;
            /* sum the previous 10 terms a(i-1) .. a(i-10) */
            for (idx_t j = i - 1; j >= i - 10; --j)
                sum10 += cache[j];
            cache[i] = sum10 * (seq_t)30 - cache[i - 11] * (seq_t)465;
        }
    }
    used = n + 1;
    return cache[n];
}
