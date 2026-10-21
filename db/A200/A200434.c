/*
 * A200434  Number of -n..n arrays x(0..6) of 7 elements with zero sum and no
 *          two or three adjacent elements summing to zero.
 * https://oeis.org/A200434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) with initial values a(1)=0, a(2)=594,
 *            a(3)=9898, a(4)=67852, a(5)=293464, a(6)=955602, a(7)=2567334
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 23, u64 n <= 144, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 0‑based entry 0 is unused */
    static const seq_t init[8] = {
        0,          /* 0 (unused) */
        0,          /* a(1) */
        594,        /* a(2) */
        9898,       /* a(3) */
        67852,      /* a(4) */
        293464,     /* a(5) */
        955602,     /* a(6) */
        2567334     /* a(7) */
    };

    if (n < 1) return 0;          /* undefined for n<1 */

    if (n <= 7) return init[n];   /* return directly from the seed values */

    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t   cap   = 0;     /* allocated size (1‑based) */
    static idx_t   last  = 0;     /* largest index already stored */

    if (cache == NULL) {
        /* allocate space for indices 0…7 (size 8) */
        cap   = 8;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        if (!cache) return 0;                 /* allocation failure */
        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = init[i];
        last  = 7;
    }

    /* enlarge cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* if the requested index is already known, return it */
    if (n <= last) return cache[n];

    /* compute missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* compute in two parts to avoid unsigned underflow */
        seq_t pos = cache[i-1] * (seq_t)7  +  /* 7*a(i-1)   */
                    cache[i-3] * (seq_t)35 + /* 35*a(i-3)  */
                    cache[i-5] * (seq_t)21 + /* 21*a(i-5)  */
                    cache[i-7];              /* + a(i-7)   */
        seq_t neg = cache[i-2] * (seq_t)21 +  /* 21*a(i-2)  */
                    cache[i-4] * (seq_t)35 +  /* 35*a(i-4)  */
                    cache[i-6] * (seq_t)7;    /* 7*a(i-6)   */
        cache[i] = pos - neg;
    }

    last = n;
    return cache[n];
}
