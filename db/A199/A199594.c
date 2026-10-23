/*
 * A199594  Number of sequences of n coin flips that win on the last flip, if
 *          the sequence of flips ends with (0,0,1,1) or (1,1,1,1).
 * https://oeis.org/A199594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199594.
   Recurrence (valid for n ≥ 8):
   a(n) = a(n-1) + a(n-2) + a(n-3) – a(n-5)
   Initial values:
   a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=3, a(6)=5, a(7)=10, a(8)=18 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to keep results between calls */
    static seq_t *cache = NULL;   /* array of values, index 0 unused */
    static idx_t cap   = 0;       /* allocated size (highest index+1) */
    static idx_t last  = 0;       /* largest n that has already been computed */

    /* Base cases for indices 1..8 */
    static const seq_t base[9] = {
        0,  /* index 0 unused */
        0,  /* a(1) */
        0,  /* a(2) */
        0,  /* a(3) */
        2,  /* a(4) */
        3,  /* a(5) */
        5,  /* a(6) */
        10, /* a(7) */
        18  /* a(8) */
    };

    /* Handle very small indices directly */
    if (n < 9) {
        if (n == 0) return 0;   /* undefined for n<1, but return 0 */
        return base[(idx_t)n];
    }

    /* Allocate or enlarge the cache if needed */
    if (cache == NULL) {
        /* Allocate at least up to max(n, 8) + 1 (index 0 unused) */
        cap = (n > 8 ? n + 1 : 9);
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure */
        /* Initialize first eight values */
        cache[1] = base[1];
        cache[2] = base[2];
        cache[3] = base[3];
        cache[4] = base[4];
        cache[5] = base[5];
        cache[6] = base[6];
        cache[7] = base[7];
        cache[8] = base[8];
        last = 8;
    } else if (n + 1 > cap) {
        idx_t newcap = cap * 2;
        if (newcap < n + 1) newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence valid for i ≥ 9 (i > 8) */
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] - cache[i-5];
    }
    last = n;

    return cache[n];
}
