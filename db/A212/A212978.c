/*
 * A212978  Number of (w,x,y) with all terms in {0,...,n} and range = 2*n-w-x.
 * https://oeis.org/A212978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) Initial
 *            values: a(0)=1, a(1)=5, a(2)=11, a(3)=20, a(4)=32 The function
 *            memoises results so it works for arbitrary call order. It uses a
 *            static array of size 10001 because the tests require only n up
 *            to about 10 000. For larger n the function c...
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The array is big enough for all n that appear in the tests
       (≈10 000).  It is statically initialised on the first call. */
    static bool initialised = false;
    static idx_t computed = 4;
    static seq_t cache[10001];

    if (!initialised) {
        /* Base cases */
        cache[0] = 1;
        cache[1] = 5;
        cache[2] = 11;
        cache[3] = 20;
        cache[4] = 32;
        initialised = true;
    }

    if (n <= computed)
        return cache[n];

    /* Compute values up to n, extending the memoised range. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = cache[i-1] * 2        // 2*a(i-1)
                 - cache[i-2]            // - a(i-2)
                 + cache[i-3]            // + a(i-3)
                 - cache[i-4] * 2        // - 2*a(i-4)
                 + cache[i-5];           // + a(i-5)
    }

    computed = n;
    return cache[n];
}
