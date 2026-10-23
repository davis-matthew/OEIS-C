/*
 * A164628  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * * a(n) = 15*a(n-1) +
 *            15*a(n-2) + 15*a(n-3) + * 15*a(n-4) + 15*a(n-5) + 15*a(n-6) -
 *            120*a(n-7) * * for n ≥ 8, with initial values * * a(0)=1,
 *            a(1)=17, a(2)=272, a(3)=4352, * a(4)=69632, a(5)=1114112,
 *            a(6)=17825792, a(7)=285212536 * (the terms given in A164628...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        /* start with room for a few more terms */
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* out not handled */

        /* initial values a(0) … a(7) from the sequence definition */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)17;
        cache[2] = (seq_t)272;
        cache[3] = (seq_t)4352;
        cache[4] = (seq_t)69632;
        cache[5] = (seq_t)1114112;
        cache[6] = (seq_t)17825792;
        cache[7] = (seq_t)285212536;

        cache_len = 8;
    }

    /* extend cache if needed */
    if ((size_t)n >= cache_len) {
        /* grow capacity to accommodate new terms */
        if ((size_t)n + 1 > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;      /* out of memory, give up */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms using the recurrence */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t val = 0;
            val += (seq_t)15 * cache[i-1];
            val += (seq_t)15 * cache[i-2];
            val += (seq_t)15 * cache[i-3];
            val += (seq_t)15 * cache[i-4];
            val += (seq_t)15 * cache[i-5];
            val += (seq_t)15 * cache[i-6];
            val -= (seq_t)120 * cache[i-7];
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
