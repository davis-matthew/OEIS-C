/*
 * A220339  Equals two maps: number of nX2 binary arrays indicating the
 *          locations of corresponding elements equal to exactly two of their
 *          king-move neighbors in a random 0..3 nX2 array.
 * https://oeis.org/A220339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: A220339 computed with its empirical linear recurrence of order 22.
 * a(1) = 1, a(2) = 5, ..., a(22) = 6542452940294
 * Recurrence for n > 22:
 * a(n) = 4*a(n-1)+6*a(n-2)-22*a(n-3)-15*a(n-4)+10*a(n-5)+56*a(n-6)+34*a(n-7)
 *        -68*a(n-8)-12*a(n-9)-247*a(n-10)+258*a(n-11)+429*a(n-12)-176*a(n-13)
 *        -798*a(n-14)+50*a(n-15)+1112*a(n-16)-368*a(n-17)-628*a(n-18)
 *        +288*a(n-19)-56*a(n-20)-248*a(n-21)+96*a(n-22)
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
// C++ build: use the operators provided by the big integer class
static inline seq_t mul_safe(seq_t t, int c)
{
    return t * (seq_t)c;
}
#else
// C build: use unsigned 128-bit multiplication to avoid overflow
static inline seq_t mul_safe(seq_t t, int c)
{
    return (seq_t)((unsigned __int128)t * (unsigned __int128)c);
}
#endif

seq_t a(idx_t n)
{
    static const seq_t init[22] = {
        1, 5, 24, 89, 389, 1570, 6413, 25744, 103229, 412081,
        1643706, 6548560, 26080372, 103836793, 413362985, 1645415613,
        6549395074, 26068520679, 103759005831, 412983036406,
        1643754108417, 6542452940294
    };
    static const int coeff[22] = {
        4, 6, -22, -15, 10, 56, 34, -68, -12, -247,
        258, 429, -176, -798, 50, 1112, -368, -628,
        288, -56, -248, 96
    };
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n <= cache_len) {
        return cache[n-1];
    }

    if (cache_len == 0) {
        cache_len = 22;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 22; ++i) {
            cache[i] = init[i];
        }
    }

    if (n > cache_len) {
        seq_t *newptr = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!newptr) {
            // allocation failed; return 0 or handle error as needed
            return 0;
        }
        cache = newptr;
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;
            for (int k = 0; k < 22; ++k) {
                idx_t j = i - (k + 1);
                seq_t term = cache[j];
                if (coeff[k] >= 0) {
                    pos += mul_safe(term, coeff[k]);
                } else {
                    neg += mul_safe(term, -coeff[k]);
                }
            }
            cache[i] = pos - neg; // pos >= neg by recurrence definition
        }
        cache_len = n;
    }

    return cache[n-1];
}
