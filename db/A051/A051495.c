/*
 * A051495  Expansion of (1 + x + 2*x^2 - x^3 + x^4)/(1 - 3*x^3 + 3*x^6 -
 *          x^9).
 * https://oeis.org/A051495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (1 - 3*x^3 + 3*x^6 - x^9) * sum_{n≥0} a(n) x^n = 1 + x + 2*x^2 -
 *            x^3 + x^4
 * Range:     u8 n <= 45, u16 n <= 765, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) satisfies: a(n) = 3*a(n-3) - 3*a(n-6) + a(n-9) + b(n),
   where b(0)=1, b(1)=1, b(2)=2, b(3)=-1, b(4)=1, otherwise 0. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cached values of the sequence */
    static seq_t *cache = NULL;
    static idx_t size = 0;

    if (n >= size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed – unlikely in test harness */
        cache = tmp;

        for (idx_t i = size; i <= n; ++i) {
            long long bn = 0;
            switch (i) {
                case 0: bn =  1; break;
                case 1: bn =  1; break;
                case 2: bn =  2; break;
                case 3: bn = -1; break;
                case 4: bn =  1; break;
                default: bn =  0; break;
            }
            long long val = bn;
            if (i >= 3)   val += 3LL * (long long)cache[i - 3];
            if (i >= 6)   val -= 3LL * (long long)cache[i - 6];
            if (i >= 9)   val += (long long)cache[i - 9];
            cache[i] = (seq_t)val;
        }
        size = n + 1;
    }
    return cache[n];
}
