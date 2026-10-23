/*
 * A170464  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1≤n<45: a(n)=2+21*∑_{i=0}^{n-1} a(i); for n=45:
 *            a(45)=21*∑_{i=1}^{44} a(i)-231*a(0)+1; for n>45:
 *            a(n)=21*∑_{i=n-44}^{n-1} a(i)-231*a(n-45);
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generated from the rational G.F. of A170464. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;        /* number of terms stored */

    /* running sums for the recurrence */
    static seq_t sumAll    = 0;  /* sum of a(0)..a(idx-1) */
    static seq_t sumLast44 = 0;  /* sum of the last 44 terms */
    /* circular buffer to keep the last 44 terms */
    static seq_t  buffer[44];
    static size_t buffer_start = 0;
    static size_t buffer_len   = 0;   /* 0..44 */

    /* enlarge cache if needed */
    if (cache_len <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(1);            /* allocation failed */
        cache    = new_ptr;
        cache_cap= new_cap;

        /* compute terms from current cache_len up to n */
        for (size_t idx = cache_len; idx <= (size_t)n; ++idx) {
            seq_t term;
            if (idx == 0) {
                term = 1;                                   /* a(0)=1 */
            } else if (idx < 45) {
                term = 2 + 21 * sumAll;                     /* a(idx) = 2 + 21*sumAll */
            } else if (idx == 45) {
                term = 21 * sumLast44 - 231 * cache[0] + 1;  /* special case for n=45 */
            } else {
                term = 21 * sumLast44 - 231 * cache[idx - 45];
            }

            /* store the term */
            cache[idx] = term;
            ++cache_len;

            /* update the running sums */
            sumAll += term;

            /* update the circular buffer and sum of last 44 terms */
            if (buffer_len < 44) {
                size_t pos = (buffer_start + buffer_len) % 44;
                buffer[pos] = term;
                ++buffer_len;
                sumLast44 += term;
            } else {
                seq_t oldest = buffer[buffer_start];
                sumLast44 += term - oldest;
                buffer[buffer_start] = term;
                buffer_start = (buffer_start + 1) % 44;
            }
        }
    }
    return cache[(size_t)n];
}
