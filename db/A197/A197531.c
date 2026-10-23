/*
 * A197531  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,0,1,0,0
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: 
   a(n) = 3*a(n-1) - a(n-3) - 4*a(n-4) - 3*a(n-5) + a(n-7) + a(n-8)  (n > 8)
   Initial values (n = 1 .. 8):
   3, 9, 16, 33, 73, 160, 361, 835
   Offset: 1   (so a(1) is the first term listed above) */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static buffer for memoization */
    static seq_t *buf = NULL;      /* 1‑based storage, buf[0] unused */
    static size_t cap = 0;         /* allocated size */
    static size_t len = 0;         /* next free index (buf[1..len-1] valid) */

    /* initial values for n = 1 .. 8 */
    static const seq_t init[9] = {
        0ULL,          /* dummy, 1‑based */
        3ULL, 9ULL, 16ULL, 33ULL,
        73ULL, 160ULL, 361ULL, 835ULL
    };

    /* Directly return when n is one of the first eight terms */
    if (n < 9)
        return init[(idx_t)n];

    /* ----- Ensure the buffer can hold the first eight values ----- */
    if (cap < 9) {
        size_t newcap = 9;
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        buf = tmp;
        cap = newcap;
    }

    /* Initialize the buffer with the first eight terms (if not yet done) */
    if (len < 9) {
        for (size_t i = 1; i <= 8; ++i)
            buf[i] = init[i];
        len = 9;            /* next free index is 9 */
    }

    /* ----- Expand buffer if needed for the requested n ----- */
    if (cap <= (size_t)n) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        buf = tmp;
        cap = newcap;
    }

    /* ----- Compute missing terms up to n ----- */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val = 3 * buf[i - 1];
        val -= buf[i - 3];
        val -= 4 * buf[i - 4];
        val -= 3 * buf[i - 5];
        val += buf[i - 7];
        val += buf[i - 8];
        buf[i] = val;
    }
    len = (size_t)n + 1;

    return buf[(size_t)n];
}
