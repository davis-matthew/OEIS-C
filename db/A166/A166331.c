/*
 * A166331  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=20, a(3)=80, a(4)=320, a(5)=1280,
 *            a(6)=5120, a(7)=20480, a(8)=81920, a(9)=327680, a(10)=1310720.
 *            For n ≥ 11 the recurrence is a(n) = 3 * Σ_{i=1}^{10} a(n-i) – 6
 *            * a(n-11) + [n==11]
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size of cache    */
    static size_t used  = 0;   /* first index not yet computed */

    /* initialise base values once */
    if (cache == NULL) {
        cache = (seq_t*)malloc(11 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* unlikely          */

        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)5;
        cache[2]  = (seq_t)20;
        cache[3]  = (seq_t)80;
        cache[4]  = (seq_t)320;
        cache[5]  = (seq_t)1280;
        cache[6]  = (seq_t)5120;
        cache[7]  = (seq_t)20480;
        cache[8]  = (seq_t)81920;
        cache[9]  = (seq_t)327680;
        cache[10] = (seq_t)1310720;

        used = 11;
        cap  = 11;
    }

    /* grow the cache if necessary */
    size_t required = (size_t)n + 1;
    if (required > cap) {
        seq_t *tmp = (seq_t*)realloc(cache, required * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cap   = required;
    }

    /* compute missing terms up to n */
    for (size_t i = used; i <= (size_t)n; ++i) {
        if (i < 11) continue;   /* already initialised */

        seq_t sum = (seq_t)0;
        for (size_t j = 1; j <= 10; ++j) {
            sum += cache[i - j];
        }

        /* 3 * sum */
        seq_t val = sum;
        val += sum;
        val += sum;

        /* 6 * a(i-11) */
        seq_t six = cache[i - 11];
        six += six;                /* 2 */
        six += six;                /* 4 */
        six += cache[i - 11];      /* 5 */
        six += cache[i - 11];      /* 6 */

        val -= six;
        if (i == 11) val += (seq_t)1;   /* p_n = 1 only for n = 11 */

        cache[i] = val;
    }

    used = (size_t)n + 1;
    return cache[(size_t)n];
}
