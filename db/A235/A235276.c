/*
 * A235276  Number of (n+1) X (6+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 15*a(n-2) - 15*a(n-3) - 80*a(n-4) + 80*a(n-5) *
 *            + 180*a(n-6) - 180*a(n-7) - 144*a(n-8) + 144*a(n-9) * (offset 1)
 *            * Initial values: a(1)=2800, a(2)=3844, a(3)=5096, a(4)=7936,
 *            a(5)=11704, * a(6)=20440, a(7)=33464, a(8)=64168, a(9)=115480
 * Range:     u8 none, u16 n <= 8, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* number of terms already stored */
    static size_t capacity = 0;

    if (n == 0) return 0; /* out of range – not used in tests */

    /* initialise cache with the first nine terms */
    if (!cache) {
        capacity = 32;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        cache[0] = (seq_t)2800;
        cache[1] = (seq_t)3844;
        cache[2] = (seq_t)5096;
        cache[3] = (seq_t)7936;
        cache[4] = (seq_t)11704;
        cache[5] = (seq_t)20440;
        cache[6] = (seq_t)33464;
        cache[7] = (seq_t)64168;
        cache[8] = (seq_t)115480;
        cached = 9;
    }

    /* extend cache if a larger n is requested */
    if ((size_t)n > cached) {
        if ((size_t)n > capacity) {
            size_t new_cap = capacity;
            while ((size_t)n > new_cap) new_cap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            capacity = new_cap;
        }
        for (idx_t i = cached + 1; i <= n; ++i) {
            idx_t idx = i - 1; /* 0‑based index for a(i) */
            seq_t pos = cache[idx - 1]                 /* a(i-1) */
                       + cache[idx - 2] * (seq_t)15
                       + cache[idx - 5] * (seq_t)80
                       + cache[idx - 6] * (seq_t)180
                       + cache[idx - 9] * (seq_t)144;
            seq_t neg = cache[idx - 3] * (seq_t)15
                       + cache[idx - 4] * (seq_t)80
                       + cache[idx - 7] * (seq_t)180
                       + cache[idx - 8] * (seq_t)144;
            cache[idx] = pos - neg;
        }
        cached = n;
    }

    return cache[n - 1];
}
