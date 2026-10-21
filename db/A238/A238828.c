/*
 * A238828  a(0)=0; thereafter a(n) = A238824(n-1)+A238825(n).
 * https://oeis.org/A238828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A238824(n-1) + A238825(n) (OEIS A238828)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses the order‑8 linear recurrence derived from the
   rational generating function:
   a(n) = 2 a(n-1) + 3 a(n-2) – 4 a(n-3) – 3 a(n-4)
          + 2 a(n-5) + 4 a(n-6) + 2 a(n-7) – a(n-8)  (n ≥ 8)   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached terms */
    static idx_t  cap     = 0;      /* allocated capacity (≥ needed) */
    static idx_t  last    = 0;      /* last computed index (≥0) */

    /* Ensure sufficient storage for indices 0 … n (and for the first 8 terms) */
    idx_t needed = n + 1;
    if (needed < 8) needed = 8;     /* we need space for indices 0…7 at least */

    if (cap == 0 || cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) {                 /* allocation failed – abort */
            exit(1);
        }
        cache = tmp;
        cap   = needed;
    }

    /* Initialize the first 8 base values once */
    if (last < 7) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 2;
        cache[4] = 5;
        cache[5] = 12;
        cache[6] = 28;
        cache[7] = 70;
        last = 7;                   /* at least 7 has been computed */
    }

    /* Compute missing terms up to n using the recurrence */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t val = cache[i-1];
            val += cache[i-1];                       /* 2 * a(i-1) */
            val += cache[i-2]; val += cache[i-2]; val += cache[i-2]; /* +3 * a(i-2) */
            val -= cache[i-3]; val -= cache[i-3]; val -= cache[i-3]; val -= cache[i-3]; /* -4 * a(i-3) */
            val -= cache[i-4]; val -= cache[i-4]; val -= cache[i-4];               /* -3 * a(i-4) */
            val += cache[i-5]; val += cache[i-5];                               /* +2 * a(i-5) */
            val += cache[i-6]; val += cache[i-6]; val += cache[i-6]; val += cache[i-6]; /* +4 * a(i-6) */
            val += cache[i-7]; val += cache[i-7];                               /* +2 * a(i-7) */
            val -= cache[i-8];                                               /* -1 * a(i-8) */
            cache[i] = val;
        }
        last = n;
    }

    return cache[n];
}
