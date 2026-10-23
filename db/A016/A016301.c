/*
 * A016301  Expansion of 1/((1-2*x)*(1-5*x)*(1-11*x)).
 * https://oeis.org/A016301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 87*a(n-2) + 110*a(n-3); a(0)=1, a(1)=18,
 *            a(2)=237
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <type_traits>
#endif

/* Helper that performs multiplication safely.
   For integral seq_t types of 64 bits or less it uses a 128‑bit
   intermediate to avoid overflow.  For non‑integral types
   (e.g. arbitrary‑precision integer classes) the plain
   multiplication operator is used. */
static inline seq_t mul_safe(seq_t a, unsigned int b)
{
#ifdef __cplusplus
    if constexpr (std::is_integral<seq_t>::value && sizeof(seq_t) <= 8) {
        return (seq_t)((__uint128_t)a * b);
    } else {
        return a * b;
    }
#else
    return (seq_t)((__uint128_t)a * b);
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0;                    /* allocation failure */
        cache[0] = 1;
        cache[1] = 18;
        cache[2] = 237;
    }

    if (n < cache_len) return cache[n];

    idx_t old_len = cache_len;
    cache_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    if (!tmp) return cache[old_len - 1];          /* keep old cache on failure */
    cache = tmp;

    for (idx_t i = old_len; i <= n; ++i) {
        seq_t term = mul_safe(cache[i - 1], 18);
        term += mul_safe(cache[i - 3], 110);
        term -= mul_safe(cache[i - 2], 87);
        cache[i] = term;
    }

    return cache[n];
}
