/*
 * A077845  Expansion of (1-x)^(-1)/(1-2*x-2*x^2-x^3).
 * https://oeis.org/A077845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n‑1) – a(n‑3) – a(n‑4)
   with a(0)=1, a(1)=3, a(2)=9, a(3)=26. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of computed terms */
    static seq_t *cache   = NULL;   /* dynamic array of terms               */
    static idx_t capacity = 0;      /* currently allocated size              */
    static idx_t last     = 0;      /* largest index computed so far         */
    static int   base_set = 0;      /* flag: base values 0..3 already stored */

    /* direct return for the four known initial values */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)3;
            case 2: return (seq_t)9;
            case 3: return (seq_t)26;
        }
    }

    /* ensure the array has space for indices 0..n */
    if (n + 1 > capacity) {
        idx_t newcap = (n + 1) * 2;
        if (newcap < n + 1) newcap = n + 1;           /* safety for overflow */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                    /* out of memory */
        cache   = tmp;
        capacity = newcap;
    }

    /* initialise base terms once */
    if (!base_set) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)26;
        last = 3;
        base_set = 1;
    }

    /* compute any missing terms up to n */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t t = cache[i - 1];
            t *= (seq_t)3;
            t -= cache[i - 3];
            t -= cache[i - 4];
            cache[i] = t;
        }
        last = n;
    }

    return cache[n];
}
