/*
 * A106108  Rowland's prime-generating sequence: a(1) = 7; for n > 1, a(n) =
 *          a(n-1) + gcd(n, a(n-1)).
 * https://oeis.org/A106108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7; for n > 1, a(n) = a(n-1) + gcd(n, a(n-1))
 * Range:     u8 n <= 100, u16 n <= 9049, u32 n <= 9049, u64 n <= 9049,
 *            u128 n <= 9049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation with a static cache (max index 20000).  This implements the
 * formula directly and works for any unsigned integer type or arbitrary‑
 * precision class that defines % and constructors from integer literals. */

#include <stddef.h>

/* Euclidean algorithm for seq_t values. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Largest n expected in the test suite (well above the 10000 bound). */
#define MAX_N 20000

/* Static cache: a[i] is stored at cache[i] (1‑based). */
static seq_t cache[MAX_N + 1];
static idx_t cached_up_to = 0;

/* External linkage function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n=0, harmless fallback */

    /* If n is within the preallocated range, use the cache. */
    if (n <= MAX_N)
    {
        if (n > cached_up_to)
        {
            for (idx_t i = cached_up_to + 1; i <= n; ++i)
            {
                if (i == 1)
                    cache[1] = (seq_t)7;
                else
                {
                    seq_t prev = cache[i - 1];
                    seq_t g    = seq_gcd(prev, (seq_t)i);
                    cache[i]   = prev + g;
                }
            }
            cached_up_to = n;
        }
        return cache[n];
    }

    /* For n beyond the preallocated limit, compute on the fly.  Older
     * cached values are preserved; only the tail is newly generated. */
    seq_t prev;
    if (cached_up_to >= 1)
        prev = cache[cached_up_to];
    else
        prev = (seq_t)7;

    for (idx_t i = cached_up_to + 1; i <= n; ++i)
    {
        seq_t g    = seq_gcd(prev, (seq_t)i);
        prev       = prev + g;
        if (i <= MAX_N)
            cache[i] = prev;
    }
    cached_up_to = n;
    return prev;
}
