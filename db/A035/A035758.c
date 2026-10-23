/*
 * A035758  Coordination sequence for C_21 lattice.
 * https://oeis.org/A035758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^(2n) in ((1+x)/(1-x))^21 Equivalent to:
 *            a(n) = sum_{i=0}^{min(21,2n)} C(21,i) * C(20+2n-i,20)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 17, u128 n <= 169,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

/* Pre‑compute binomial coefficients C(21,i) for i=0..21.  */
static seq_t binom21[22];
static int binom21_init = 0;
static void init_binom21(void)
{
    if (binom21_init) return;
    binom21_init = 1;
    for (int i = 0; i <= 21; ++i) {
        seq_t res = 1;
        int a = 21, b = i;
        if (b > a - b) b = a - b;            /* choose smaller side */
        for (int k = 1; k <= b; ++k) {
            seq_t num = (seq_t)(a - b + k);
            seq_t den = (seq_t)k;
            seq_t g = gcd(num, den);
            num /= g;   den /= g;
            seq_t g2 = gcd(res, den);
            res /= g2;  den /= g2;
            res *= num; /* den should be 1 now */
        }
        binom21[i] = res;
    }
}

/* Compute C(N,20) for arbitrary N >= 20, using multiplicative formula
   with cancellation to avoid overflow.  */
static seq_t binom_n_20(seq_t N)
{
    /* N >= 20 */
    seq_t res = 1;
    for (int k = 1; k <= 20; ++k) {
        seq_t num = N - 20 + k;   /* equal to N - 20 + k */
        seq_t den = (seq_t)k;
        seq_t g = gcd(num, den);
        num /= g; den /= g;
        seq_t g2 = gcd(res, den);
        res /= g2; den /= g2;
        res *= num;                /* den should now be 1 */
    }
    return res;
}

/* Memoized sequence storage.  */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Return a(n).  */
seq_t a(idx_t n)
{
    init_binom21();

    /* Expand cache if necessary.  */
    if ((size_t)n >= cache_len) {
        size_t new_len = ((size_t)n + 1) * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed, return 0 as fallback */
        cache = new_cache;
        for (size_t i = cache_len; i < new_len; ++i)
            cache[i] = 0; /* zero-fill new portion */
        cache_len = new_len;
    }

    if (cache[(size_t)n] != 0) /* memoized value exists (a(0)=1 so 0 only for uncomputed) */
        return cache[(size_t)n];

    /* Compute a(n).  */
    size_t imax = 21;
    if ((size_t)(2 * n) < imax)
        imax = (size_t)(2 * n);

    seq_t result = 0;
    for (size_t i = 0; i <= imax; ++i) {
        seq_t c21 = binom21[i];
        seq_t N = (seq_t)20 + (seq_t)2 * n - i;
        seq_t cn20 = binom_n_20(N);
        result += c21 * cn20;
    }

    cache[(size_t)n] = result;
    return result;
}
