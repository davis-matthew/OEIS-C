/*
 * A192307  0-sequence of reduction of (3n) by x^2 -> x+1.
 * https://oeis.org/A192307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192307: 0‑sequence of reduction of (3n) by x^2 -> x+1
   Recurrence: a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + a(n-4) + a(n-5), n≥6
   Initial values: a(1)=3, a(2)=3, a(3)=12, a(4)=24, a(5)=54
   (All computations use type seq_t supplied by the build.) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms, 1‑based indices stored at
       zero‑based array positions. */
    static seq_t *buf = NULL;   /* pointer to array of seq_t */
    static size_t cached = 0;   /* number of terms currently stored */
    static size_t size = 0;     /* current capacity of buf */

    /* Handle invalid (zero) index defensively. */
    if (n == 0) return 0;

    /* Initialise cache with first five terms on first call. */
    if (buf == NULL) {
        size = 5;
        buf = (seq_t *)malloc(sizeof(seq_t) * size);
        if (!buf) abort();          /* allocation failure */
        buf[0] = 3;                 /* a(1) */
        buf[1] = 3;                 /* a(2) */
        buf[2] = 12;                /* a(3) */
        buf[3] = 24;                /* a(4) */
        buf[4] = 54;                /* a(5) */
        cached = 5;
    }

    /* Return early if the requested term is already in the cache. */
    if (n <= cached) {
        return buf[n - 1];
    }

    /* Grow the buffer if necessary. */
    if ((size_t)n > size) {
        size_t newsize = size;
        while (newsize < (size_t)n) newsize <<= 1;   /* double strategy */
        seq_t *tmp = (seq_t *)realloc(buf, sizeof(seq_t) * newsize);
        if (!tmp) abort();                          /* allocation failure */
        buf = tmp;
        size = newsize;
    }

    /* Compute and store terms from cached+1 up to n using the recurrence. */
    for (idx_t i = cached + 1; i <= n; ++i) {
        /* Terms needed for the recurrence:
           a_{i-1} -> buf[i-2]
           a_{i-2} -> buf[i-3]
           a_{i-3} -> buf[i-4]
           a_{i-4} -> buf[i-5]
           a_{i-5} -> buf[i-6]   (i ≥ 6, guaranteed by loop start) */
        seq_t val = 3 * buf[i - 2];   /* 3*a(i-1) */
        val -= buf[i - 3];           /* -a(i-2)   */
        val -= 3 * buf[i - 4];       /* -3*a(i-3) */
        val += buf[i - 5];           /* +a(i-4)   */
        val += buf[i - 6];           /* +a(i-5)   */
        buf[i - 1] = val;
    }

    cached = n;
    return buf[n - 1];
}
