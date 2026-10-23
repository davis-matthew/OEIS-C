/*
 * A167649  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A167649: number of reduced words of length n.
   Initial values: a(0)=1, a(1)=4, a(2)=12, a(3)=36, a(4)=108,
   a(5)=324, a(6)=972, a(7)=2916, a(8)=8748, a(9)=26244,
   a(10)=78732, a(11)=236196, a(12)=708588, a(13)=2125764,
   a(14)=6377292.
   For n ≥ 15:
   a(n) = 2*(a(n-1)+…+a(n-14)) – 3*a(n-15),   except for n = 15
   where a(15) = 2*(a(14)+…+a(1)) – 3*a(0) + 1.  (The +1 comes from
   the constant term of the generating function.)  */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system.  */
seq_t a(idx_t n)
{
    /* first 15 terms are known a priori */
    static const seq_t init[15] = {
        (seq_t)1,     (seq_t)4,     (seq_t)12,    (seq_t)36,
        (seq_t)108,   (seq_t)324,   (seq_t)972,   (seq_t)2916,
        (seq_t)8748,  (seq_t)26244, (seq_t)78732, (seq_t)236196,
        (seq_t)708588,(seq_t)2125764,(seq_t)6377292
    };

    /* cache of already computed terms */
    static seq_t *cache = NULL;      /* dynamic array */
    static size_t cap   = 0;         /* current capacity */
    static size_t last  = 0;         /* highest index that has been computed */

    /* handle the trivial cases */
    if (n < 15)
        return init[(size_t)n];

    /* allocate and initialise the cache if it doesn't exist yet */
    if (!cache) {
        cap = 15;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();                 /* allocation failed */
        for (size_t i = 0; i < 15; ++i)
            cache[i] = init[i];
        last = 14;                           /* indices 0..14 are known */
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 15;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* reallocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute any missing terms up to n */
    if (last < (size_t)n) {
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            seq_t sum = 0;
            /* 2 * a(i-k) for k = 1 .. 14 */
            for (int k = 1; k <= 14; ++k)
                sum += (seq_t)2 * cache[i - k];
            /* subtract 3 * a(i-15) */
            sum -= (seq_t)3 * cache[i - 15];
            /* special RHS +1 for i == 15 */
            if (i == 15)
                sum += (seq_t)1;
            cache[i] = sum;
        }
        last = (size_t)n;
    }

    return cache[(size_t)n];
}
