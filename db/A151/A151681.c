/*
 * A151681  a(n) = sum_{k >= 0, k even} binomial(wt(n+k),k+1), where wt() =
 *          A000120().
 * https://oeis.org/A151681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=0, k even} binomial(A000120(n + k), k)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Fast popcount for idx_t (works for uint64_t). */
static inline unsigned int popcount_u64(idx_t x)
{
#if defined(__has_builtin)
# if __has_builtin(__builtin_popcountll)
    return __builtin_popcountll(x);
# endif
#endif
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x)
    {
        x &= x - 1;
        c++;
    }
    return c;
#endif
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; ; k += 2)
    {
        idx_t nk = n + k;                /* n + k */
        unsigned int w = popcount_u64(nk);   /* wt(n + k) */
        if ((idx_t)w < k)                 /* binomial w choose k is zero when k > w */
            break;
        sum += binomial((idx_t)w, k);     /* binomial(wt(n + k), k) */
    }
    return sum;
}
