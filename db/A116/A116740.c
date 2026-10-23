/*
 * A116740  Number of permutations of length n which avoid the patterns 1243,
 *          2341, 4132.
 * https://oeis.org/A116740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   _max_sz, _min_sz
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=73,
   a(6)=239, a(7)=740, a(8)=2199, a(9)=6348
   a(n) = 12*a(n-1) - 62*a(n-2) + 181*a(n-3) - 329*a(n-4)
          + 386*a(n-5) - 292*a(n-6) + 137*a(n-7) - 36*a(n-8) + 4*a(n-9)
          (n ≥ 10) */

#include <stdlib.h>
#include "../utils/_max_sz.h"
#include "../utils/_min_sz.h"

seq_t a(idx_t n)
{
    /* Handle out‑of‑bounds request */
    if (n < 1) return 0;

    static seq_t *cache = NULL;   /* array a[0..cap] */
    static size_t cap = 0;        /* allocated size - 1 */
    static size_t computed = 0;   /* highest index computed */

    /* Ensure storage is large enough */
    if (n > cap) {
        size_t new_cap = cap == 0 ? 16 : cap << 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();               /* memory failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms */
    if (n > computed) {
        /* Initial 1..9 */
        if (computed < 9) {
            static const seq_t init[10] = {0,1,2,6,21,73,239,740,2199,6348};
            size_t start = computed + 1;
            size_t end   = _min_sz((size_t)9, n);
            for (size_t i = start; i <= end; ++i) {
                cache[i] = init[i];
            }
            computed = _max_sz(computed, end);
        }

        /* Recurrence for i ≥ 10 */
        for (size_t i = _max_sz(computed + 1, (size_t)10); i <= n; ++i) {
            /* Positive contributions */
            seq_t pos = cache[i-1] * (seq_t)12;
            pos += cache[i-3] * (seq_t)181;
            pos += cache[i-5] * (seq_t)386;
            pos += cache[i-7] * (seq_t)137;
            pos += cache[i-9] * (seq_t)4;

            /* Negative contributions */
            seq_t neg = cache[i-2] * (seq_t)62;
            neg += cache[i-4] * (seq_t)329;
            neg += cache[i-6] * (seq_t)292;
            neg += cache[i-8] * (seq_t)36;

            cache[i] = pos - neg;
        }
        computed = n;
    }

    return cache[n];
}
