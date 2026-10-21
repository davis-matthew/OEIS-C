/*
 * A066178  Nonzero heptanacci numbers, A122189.
 * https://oeis.org/A066178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7), * a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=4, a(4)=8, a(5)=16, a(6)=32, a(7)=64. *
 *            This implements the Nonzero heptanacci numbers, A122189
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache to store computed terms. Size is sufficient for n up to 20000. */
    static seq_t cache[20001];
    static size_t cache_len = 0;   /* number of terms currently stored */

    /* initial values for indices 0 .. 7 */
    static const seq_t init[8] = {1, 1, 2, 4, 8, 16, 32, 64};

    size_t idx = (size_t)n;

    /* initialise cache on first call */
    if (cache_len == 0) {
        for (size_t i = 0; i < 8; ++i) {
            cache[i] = init[i];
        }
        cache_len = 8;
    }

    /* if we already have the requested term, return it immediately */
    if (idx < cache_len) {
        return cache[idx];
    }

    /* compute terms up to idx */

    /* if the index is within the preallocated buffer, just extend the cache */
    if (idx < 20001) {
        for (size_t k = cache_len; k <= idx; ++k) {
            seq_t sum = 0;
            for (size_t j = 1; j <= 7; ++j) {
                sum += cache[k - j];
            }
            cache[k] = sum;
        }
        cache_len = idx + 1;
        return cache[idx];
    }

    /* index is beyond the preallocated buffer.  Use a sliding window to
       compute the value on the fly without storing all intermediate terms.  */

    /* initialise a 7‑term ring buffer with the last 7 computed values
       (indices 19994 .. 20000).  These are needed to start the recurrence
       beyond the cached range.  */
    seq_t ring[7];
    size_t start = 19994;          /* 20001 - 7 */
    for (size_t j = 0; j < 7; ++j) {
        ring[j] = cache[start + j];
    }

    /* compute successive values until reaching 'idx' */
    seq_t next = 0;
    for (size_t j = 0; j < 7; ++j) {
        next += ring[j];
    }

    for (size_t k = 20001; k <= idx; ++k) {
        if (k == idx) {
            return next;
        }
        /* shift ring left, insert 'next' at the end */
        for (size_t s = 0; s < 6; ++s) {
            ring[s] = ring[s + 1];
        }
        ring[6] = next;

        /* compute following term */
        seq_t sum = 0;
        for (size_t s = 0; s < 7; ++s) {
            sum += ring[s];
        }
        next = sum;
    }

    /* this point is never reached */
    return 0;
}
