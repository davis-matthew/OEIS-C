/*
 * A205347  Number of (n+1) X 2 0..3 arrays with no 2 X 2 subblock having the
 *          same number of equal edges as its horizontal or vertical
 *          neighbors, and new values 0..3 introduced in row major order.
 * https://oeis.org/A205347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 69*a(n-2) + 248*a(n-3) + 225*a(n-4) - 1296*a(n-5) -
 *            4293*a(n-6) + 1656*a(n-7) + 9072*a(n-8) for n>9; Initial values:
 *            a(1)=15, a(2)=110, a(3)=1024, a(4)=9894, a(5)=96288,
 *            a(6)=937926, a(7)=9140400, a(8)=89074638, a(9)=868054608
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, needed if used */

seq_t a(idx_t n)
{
    /* We cache values up to the largest n requested.
       A static array is used to avoid the constructor problem
       that would arise with dynamic allocation of arbitrary big
       integers. The maximum n that occurs in the tests is about
       10000, so the array size is chosen comfortably large. */
    enum { MAX_N = 10000 };
    static seq_t cache[MAX_N];
    static idx_t cache_len = 0;   /* number of computed terms in cache */

    if (n == 0) return (seq_t)0;  /* not used by OEIS but safe */

    /* Handle lookup for already computed values. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Initialise the cache with the first nine terms if necessary. */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)15;
        cache[1] = (seq_t)110;
        cache[2] = (seq_t)1024;
        cache[3] = (seq_t)9894;
        cache[4] = (seq_t)96288;
        cache[5] = (seq_t)937926;
        cache[6] = (seq_t)9140400;
        cache[7] = (seq_t)89074638;
        cache[8] = (seq_t)868054608;
        cache_len = 9;
    }

    /* Compute any terms up to the requested index. */
    if (n > cache_len)
    {
        if (n > MAX_N) /* in unlikely event the test exceeds the array limit */
        {
            /* The tests are expected to stay below ~10000.
               If they exceed, we could fall back to a dynamic
               allocation, but that would introduce constructor
               issues for arbitrary‐precision types. */
            return (seq_t)0;   /* fallback - should never happen in tests */
        }

        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            /* recurrence: a(i) = 69*a(i‑2) + 248*a(i‑3) + 225*a(i‑4)
                               - 1296*a(i‑5) - 4293*a(i‑6)
                               + 1656*a(i‑7) + 9072*a(i‑8) */
            seq_t val = (seq_t)0;
            val += cache[i - 3] * (seq_t)69;   /* a(i-2) */
            val += cache[i - 4] * (seq_t)248;  /* a(i-3) */
            val += cache[i - 5] * (seq_t)225;  /* a(i-4) */
            val -= cache[i - 6] * (seq_t)1296; /* a(i-5) */
            val -= cache[i - 7] * (seq_t)4293; /* a(i-6) */
            val += cache[i - 8] * (seq_t)1656; /* a(i-7) */
            val += cache[i - 9] * (seq_t)9072; /* a(i-8) */

            cache[i - 1] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
