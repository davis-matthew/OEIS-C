/*
 * A164695  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=50, a(2)=2450, a(3)=120050, a(4)=5882450,
 *            a(5)=288240050, a(6)=14123762450. For n>=7: a(n) =
 *            48*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)) - 1176*a(n-7) +
 *            (n==7 ? 1 : 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ensure you have a proper definition of seq_t and idx_t in the build. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 7 initial values (0..6).  a(7) is computed specially because of the
       "+1" term coming from the numerator of the generating function. */
    static const seq_t init6[7] = {
        (seq_t)1ULL,            /* a(0) */
        (seq_t)50ULL,           /* a(1) */
        (seq_t)2450ULL,         /* a(2) */
        (seq_t)120050ULL,       /* a(3) */
        (seq_t)5882450ULL,      /* a(4) */
        (seq_t)288240050ULL,    /* a(5) */
        (seq_t)14123762450ULL   /* a(6) */
    };

    static seq_t *cache   = NULL;   /* a(0..max_computed) */
    static idx_t  cache_len = 0;    /* allocated length */
    static idx_t  computed  = 0;    /* largest index computed */

    /* Allocate or grow the cache on first request or when we need more space. */
    if (cache == NULL) {
        cache_len = (n + 1 > 8) ? (n + 1) : 8;   /* at least 0..7 will be stored */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        /* initialise entries to zero (not strictly needed) */
        for (idx_t i = 0; i < cache_len; ++i) cache[i] = (seq_t)0;
        /* copy known initial terms */
        for (idx_t i = 0; i < 7 && i < cache_len; ++i) cache[i] = init6[i];

        /* compute a(7) with the extra +1 constant */
        seq_t term = (seq_t)0;
        /* sum 48*a(1)..48*a(6) */
        for (idx_t i = 1; i <= 6; ++i) term += (seq_t)48 * cache[i];
        /* subtract 1176*a(0) */
        term -= (seq_t)1176 * cache[0];
        /* add the numerator contribution */
        term += (seq_t)1;
        cache[7] = term;

        computed = 7;
    } else if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        /* zero new slots */
        for (idx_t i = cache_len; i < new_len; ++i) cache[i] = (seq_t)0;
        cache_len = new_len;
    }

    /* compute missing terms up to n */
    if (n > computed) {
        for (idx_t k = computed + 1; k <= n; ++k) {
            seq_t term = (seq_t)0;
            /* sum 48*a(k-1) .. 48*a(k-6) */
            for (idx_t i = 1; i <= 6; ++i) term += (seq_t)48 * cache[k - i];
            /* subtract 1176*a(k-7) */
            term -= (seq_t)1176 * cache[k - 7];
            cache[k] = term;
        }
        computed = n;
    }

    return cache[n];
}
