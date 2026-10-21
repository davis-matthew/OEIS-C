/*
 * A213044  Convolution of Fibonacci numbers and positive integers repeated
 *          three times (A000045 and A008620).
 * https://oeis.org/A213044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-1)+a(n-2)+2*a(n-3)-2*a(n-4)-2*a(n-5)-a(n-6)+a(n-7)+a(n-8)
 *            for n≥8, a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=7, a(5)=12,
 *            a(6)=22, a(7)=34
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t INIT[8] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)5,
                               (seq_t)7, (seq_t)12, (seq_t)22, (seq_t)34 };

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;   /* number of values stored */

    /* initialise cache with the first 8 terms */
    if (!cache) {
        size_t init_cap = 64;
        if (init_cap < 8) init_cap = 8;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        for (idx_t i = 0; i < 8; ++i) cache[i] = INIT[i];
        cache_size = 8;
    }

    /* grow cache if request exceeds its capacity */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure – unlikely for n<10000 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n using the recurrence */
    if ((size_t)n >= cache_size) {
        for (size_t k = cache_size; k <= (size_t)n; ++k) {
            seq_t pos = cache[k-1] + cache[k-2]
                        + (cache[k-3] * ((seq_t)2))
                        + cache[k-7] + cache[k-8];
            seq_t neg = (cache[k-4] * ((seq_t)2))
                        + (cache[k-5] * ((seq_t)2))
                        + cache[k-6];
            cache[k] = pos - neg;
        }
        cache_size = (size_t)n + 1;
    }

    /* return the requested term */
    if (n < 8) return INIT[n];
    return cache[n];
}
