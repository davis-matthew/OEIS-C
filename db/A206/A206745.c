/*
 * A206745  Number of (n+1) X 2 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having an equal number of clockwise and counterclockwise edge
 *          increases.
 * https://oeis.org/A206745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 55*a(n-2) + 17*a(n-3) + 108*a(n-4) -
 *            84*a(n-5) initial values: a(1)=256, a(2)=1888, a(3)=15680,
 *            a(4)=139744, a(5)=1289472
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

extern
seq_t a(idx_t n)
{
    /* 1‑based sequence, 0‑based cache */
    static seq_t *cache = NULL;          /* pointer to cached values   */
    static idx_t cache_len = 0;          /* number of terms cached     */

    /* initial terms, in order a(1), ..., a(5) */
    static const seq_t init_terms[5] = {
        256,          /* a(1) */
        1888,         /* a(2) */
        15680,        /* a(3) */
        139744,       /* a(4) */
        1289472       /* a(5) */
    };

    /* a(0) is undefined – return 0 for safety */
    if (n == 0) {
        return 0;
    }

    /* extend the cache if necessary */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL && new_len > 0) {
            abort();               /* allocation failure */
        }
        cache = tmp;

        /* initialise the first five terms if not yet done */
        idx_t i = 0;
        while (i < new_len && i < 5) {
            cache[i] = init_terms[i];
            ++i;
        }

        /* compute remaining terms using the recurrence */
        for (; i < new_len; ++i) {
            /* the operations are performed in seq_t arithmetic;
               for arbitrary-precision integers they are exact,
               for unsigned small integers the overflow is modulo 2^k,
               which is acceptable for non‑checked values. */
            seq_t t1 = cache[i-1] * 15;
            seq_t t2 = cache[i-2] * 55;
            seq_t t3 = cache[i-3] * 17;
            seq_t t4 = cache[i-4] * 108;
            seq_t t5 = cache[i-5] * 84;

            cache[i] = t1 + t3 + t4 - t2 - t5;
        }

        cache_len = new_len;
    }

    return cache[n-1];  /* sequence is 1‑based */
}
