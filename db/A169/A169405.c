/*
 * A169405  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is defined by the rational generating function (1 +
 *            2∑_{k=1}^{31}t^k + t^32)/(1 – 6∑_{k=1}^{31}t^k + 21t^32).
 *            Equivalently, a(0)=1, a(1)=…=a(31)=2, a(32)=1, and for n≥33 a(n)
 *            = 6·(a(n–1)+…+a(n–31)) – 21·a(n–32)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t   last  = 0;   /* highest computed index */
    
    /* initialise cache for absolute safety */
    if (cache == NULL) {
        cap   = (size_t)(n > 32 ? n + 1 : 64);   /* start with some space */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();              /* insufficient memory */
        /* first value */
        cache[0] = 1;
        last = 0;
    }
    
    /* expand capacity if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap   = newcap;
    }
    
    /* if requested term already computed, return it */
    if (n <= last) return cache[n];
    
    /* compute terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t m = 31;
        if (i < 31) m = (idx_t)(i);  /* maximum number of previous terms to sum */
        for (idx_t j = 1; j <= m; ++j) sum += cache[i - j];
        
        seq_t prev32 = 0;
        if (i >= 32) prev32 = cache[i - 32];
        
        seq_t term = 0;
        if (i == 0)        term = 1;
        else if (i <= 31)  term = 2;
        else if (i == 32)  term = 1;
        /* else term stays 0 for i>32 */
        
        cache[i] = term + (seq_t)6 * sum - (seq_t)21 * prev32;
    }
    
    last = n;
    return cache[n];
}
