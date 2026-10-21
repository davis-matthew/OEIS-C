/*
 * A099581  a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,k-1)*3^(n-k-1).
 * https://oeis.org/A099581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,k-1)*3^(n-k-1).
 *            Recurrence: a(n) = 3*a(n-1) + 6*a(n-2) - 9*a(n-3) - 9*a(n-4)
 *            Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=3
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, NULL */
#include <stddef.h>   /* for size_t */

#if defined(__cplusplus)
#include <type_traits>
#endif

/* Multiply a seq_t by a small unsigned integer without overflow.
   For integral seq_t we use unsigned __int128. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
#if defined(__cplusplus)
    if constexpr (std::is_integral<seq_t>::value)
        return (seq_t)((unsigned __int128)x * m);
    else
        return x * m;
#else
    return (seq_t)((unsigned __int128)x * m);
#endif
}

/* Static cache for memoised values. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;    /* number of elements currently cached */

seq_t a(idx_t n)
{
    /* Grow cache if necessary. */
    if (n >= cache_size) {
        size_t need = (size_t)n + 1U;
        seq_t *p = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!p) {
            /* Allocation failure: abort. */
            abort();
        }
        cache = p;
        /* Compute values from current cache_size up to n. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            switch (i) {
                case 0:
                case 1:
                    val = 0;
                    break;
                case 2:
                    val = 1;
                    break;
                case 3:
                    val = 3;
                    break;
                default:
                    val = mul_small(cache[i-1], 3);
                    val += mul_small(cache[i-2], 6);
                    val -= mul_small(cache[i-3], 9);
                    val -= mul_small(cache[i-4], 9);
                    break;
            }
            cache[i] = val;
        }
        cache_size = need;
    }
    return cache[n];
}
