/*
 * A164403  Number of binary strings of length n with no substrings equal to
 *          0010 or 1001.
 * https://oeis.org/A164403
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + a(n-5) for n>=9 initial values:
 *            a(4)=14, a(5)=25, a(6)=45, a(7)=80, a(8)=143
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 76, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* for n < 4 the sequence is not defined – return 0 */
    if (n < 4) return 0;

    static seq_t *cache = NULL;   /* stores a(4) .. a(max) */
    static idx_t last = 3;        /* highest n that has been computed */
    static size_t size = 0;       /* allocated number of cache elements */

    /* first-time initialization: store the five known terms */
    if (cache == NULL)
    {
        size = 5;                                 /* a(4) .. a(8) */
        cache = (seq_t*)malloc(sizeof(seq_t) * size);
        if (!cache) return 0;                     /* allocation failed */
        cache[0] = (seq_t)14;                     /* a(4) */
        cache[1] = (seq_t)25;                     /* a(5) */
        cache[2] = (seq_t)45;                     /* a(6) */
        cache[3] = (seq_t)80;                     /* a(7) */
        cache[4] = (seq_t)143;                    /* a(8) */
        last   = 8;
    }

    /* if requested value already cached, return it */
    if (n <= last) return cache[(size_t)(n - 4)];

    /* enlarge cache if necessary */
    size_t new_len = (size_t)(n - 3);         /* number of terms a(4)..a(n) */
    if (new_len > size)
    {
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return 0;                  /* allocation failed */
        cache = tmp;
        size  = new_len;
    }

    /* compute new terms from last+1 up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t val;
        if (i >= 9)
        {
            /* a(i) = 2*a(i-1) - a(i-3) + a(i-5) */
            val = 2 * cache[(size_t)(i - 5)]   /* a(i-1) */
                - cache[(size_t)(i - 7)]      /* a(i-3) */
                + cache[(size_t)(i - 9)];     /* a(i-5) */
        }
        else
        {
            /* for i = 4..8 the values are already in cache */
            continue;
        }
        cache[(size_t)(i - 4)] = val;
    }
    last = n;
    return cache[(size_t)(n - 4)];
}
