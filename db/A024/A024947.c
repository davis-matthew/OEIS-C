/*
 * A024947  a(n) = Sum_{i=1..floor((n+2)/4)} a(2i-1)*a(n-2i+1), with
 *          a(1)=a(2)=1 and a(3)=3.
 * https://oeis.org/A024947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..floor((n+2)/4)} a(2i-1)*a(n-2i+1), with
 *            a(1)=a(2)=1, a(3)=3
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[1] holds a(1) … */
    static size_t  cap   = 0;     /* allocated size of cache  */
    static idx_t   upto  = 0;     /* largest index already computed */

    if (n < 1) return 0;

    /* Grow cache if required. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t k = upto + 1; k <= n; ++k) {
        if (k == 1 || k == 2) {
            cache[k] = (seq_t)1;
        } else if (k == 3) {
            cache[k] = (seq_t)3;
        } else {
            seq_t sum = 0;
            idx_t limit = (k + 2) / 4;
            for (idx_t i = 1; i <= limit; ++i) {
                idx_t left  = 2 * i - 1;
                idx_t right = k - 2 * i + 1;
                sum += cache[left] * cache[right];
            }
            cache[k] = sum;
        }
    }
    upto = n;
    return cache[n];
}
