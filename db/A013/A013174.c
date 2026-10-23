/*
 * A013174  exp(arctanh(x) + arctan(x)) = 1 + 2*x + 4/2!*x^2 + 8/3!*x^3 +
 *          16/4!*x^4 + 80/5!*x^5 +...
 * https://oeis.org/A013174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * for n ≥ 1, * a(n) = 2 · Σ_{k≡1 (mod 4), 1≤k≤n} a(n‑k)
 *            · (n‑1)! / (n‑k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* external integer types supplied by the build system */
static seq_t *cache = NULL;   /* holds a[0..computed]                */
static idx_t  computed = 0;   /* highest index that has been computed */

/* compute a(n) and store all intermediate values */
seq_t a(idx_t n)
{
    /* allocate and initialise for the first call */
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        computed   = 0;
    }

    /* grow the cache if necessary */
    if (n > computed) {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) exit(1);
        /* zero any newly allocated slots (optional, but harmless) */
        for (idx_t i = computed + 1; i <= n; ++i) tmp[i] = 0;
        cache  = tmp;
        /* compute values from computed+1 up to n */
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t sum  = 0;
            seq_t prod = 1;          /* prod = Π_{t=1}^{k-1} (i‑t) for current k */
            idx_t k = 1;             /* first term uses k = 1 */
            while (k <= i) {
                /* term a[i‑k] multiplied by the factorial ratio prod */
                sum += cache[i - k] * prod;
                /* prepare prod for the next valid k (+4) */
                if (k + 4 <= i) {
                    prod *= (seq_t)(i - k);
                    prod *= (seq_t)(i - k - 1);
                    prod *= (seq_t)(i - k - 2);
                    prod *= (seq_t)(i - k - 3);
                }
                k += 4;
            }
            cache[i] = sum * (seq_t)2;
        }
        computed = n;
    }
    return cache[n];
}
