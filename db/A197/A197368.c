/*
 * A197368  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,4,1,0,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A197368
   Recurrence: a(n) = 2*a(n‑1) + 2*a(n‑2) + 7*a(n‑3) – 3*a(n‑4)
                – 7*a(n‑6) – 5*a(n‑7) + 3*a(n‑8)
   Initial values (n = 1 … 8):
     a(1)=1, a(2)=3, a(3)=32, a(4)=45, a(5)=189,
     a(6)=646, a(7)=1873, a(8)=6209
   The sequence is memoized so that calls can arrive in arbitrary order. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based array of terms, cache[0] unused */
static size_t stored = 0;     /* highest n for which cache[n] is computed */
static size_t cap    = 0;     /* allocated capacity (index 0 … cap-1) */

/* initialise cache with the first 8 terms */
static void init_cache(void)
{
    if (cache == NULL) {
        cap = 9;                                 /* indices 0 … 8 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            exit(1);                              /* allocation failure */

        cache[0] = (seq_t)0;                     /* dummy, not used */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)32;
        cache[4] = (seq_t)45;
        cache[5] = (seq_t)189;
        cache[6] = (seq_t)646;
        cache[7] = (seq_t)1873;
        cache[8] = (seq_t)6209;
        stored = 8;
    }
}

/* ensure that cache is large enough for indices up to n */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL)
            exit(1);                              /* allocation failure */
        cache = tmp;
        cap = newcap;
    }
}

/* compute a(n) using memoization */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined; return 0 for safety */

    init_cache();

    if ((size_t)n > stored) {
        ensure_capacity(n);
        for (idx_t i = (idx_t)stored + 1; i <= n; ++i) {
            seq_t pos = (seq_t)2 * cache[i-1]
                       + (seq_t)2 * cache[i-2]
                       + (seq_t)7 * cache[i-3]
                       + (seq_t)3 * cache[i-8];
            seq_t neg = (seq_t)3 * cache[i-4]
                       + (seq_t)7 * cache[i-6]
                       + (seq_t)5 * cache[i-7];
            cache[i] = pos - neg;   /* pos >= neg by the recurrence */
        }
        stored = n;
    }

    return cache[n];
}
