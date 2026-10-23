/*
 * A119864  Numbers m such that the numerator of BernoulliB(m) is divisible by
 *          691.
 * https://oeis.org/A119864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 235, u32 n <= 3448, u64 n <= 3448,
 *            u128 n <= 3448, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119864:
   a(n) is the nth term of the sorted union of
   12+690*k (k≥0), 200+690*k (k≥0), and
   1382*k (k≥1), with multiples of 476790 = 690*691 omitted. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t size = 0;
    static size_t cap   = 0;

    /* current values of the three progressions */
    static seq_t next1 = 12;     /* 12 + 690*k */
    static seq_t next2 = 200;    /* 200 + 690*k */
    static seq_t next3 = 1382;   /* 2*691*k, k starts at 1 */

    /* only allocate on first use */
    if (cache == NULL) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed – return 0 */
    }

    /* generate terms until we have at least n of them */
    while (size < n) {
        /* pick the smallest of the three current candidates */
        seq_t m = next1;
        if (next2 < m) m = next2;
        if (next3 < m) m = next3;

        /* skip numbers that are multiples of 476790 */
        if ((m % (seq_t)476790) == 0) {
            if (next1 == m) next1 += (seq_t)690;
            if (next2 == m) next2 += (seq_t)690;
            if (next3 == m) next3 += (seq_t)1382;
            continue;
        }

        /* extend cache if necessary */
        if (size == cap) {
            cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failed – return 0 */
            cache = tmp;
        }

        /* store the term */
        cache[size++] = m;

        /* advance the progressions that contributed to this term */
        if (next1 == m) next1 += (seq_t)690;
        if (next2 == m) next2 += (seq_t)690;
        if (next3 == m) next3 += (seq_t)1382;
    }

    return cache[n - 1];
}
