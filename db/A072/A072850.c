/*
 * A072850  Number of permutations p of [n] satisfying i-2 <= p(i) <= i+4 for
 *          all i in [n].
 * https://oeis.org/A072850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 4*a(n-3) + 6*a(n-4) + 10*a(n-5) +
 *            12*a(n-6) - 4*a(n-7) - 6*a(n-8) - 6*a(n-9) - 2*a(n-11) -
 *            2*a(n-12) + a(n-14) + a(n-15), base values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=6, a(4)=18, a(5)=54, a(6)=146, a(7)=391, a(8)=1081,
 *            a(9)=3004, a(10)=8320, a(11)=22984, a(12)=63424, a(13)...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static const seq_t init[16] = {
    (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)6,  (seq_t)18,  (seq_t)54,
    (seq_t)146, (seq_t)391, (seq_t)1081, (seq_t)3004, (seq_t)8320,
    (seq_t)22984, (seq_t)63424, (seq_t)175176, (seq_t)484113,
    (seq_t)1337721
};

seq_t a(idx_t n)
{
    if (n < 16)
        return init[n];

    /* Initialise cache with the first 16 terms if necessary */
    if (cache_size < 16) {
        cache = (seq_t*)realloc(cache, 16 * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation failure */
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = init[i];
        cache_size = 16;
    }

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) {
            seq_t res = cache[i - 1]
                      + cache[i - 2] * (seq_t)2
                      + cache[i - 3] * (seq_t)4
                      + cache[i - 4] * (seq_t)6
                      + cache[i - 5] * (seq_t)10
                      + cache[i - 6] * (seq_t)12;
            res = res - cache[i - 7] * (seq_t)4;
            res = res - cache[i - 8] * (seq_t)6;
            res = res - cache[i - 9] * (seq_t)6;
            res = res - cache[i - 11] * (seq_t)2;
            res = res - cache[i - 12] * (seq_t)2;
            res = res + cache[i - 14];
            res = res + cache[i - 15];
            cache[i] = res;
        }
        cache_size = new_size;
    }
    return cache[n];
}
