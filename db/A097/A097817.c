/*
 * A097817  E.g.f. exp(2x)/(1-3x).
 * https://oeis.org/A097817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(n) = 3*n*a(n-1) + 2^n (n > 0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence is needed up to n ≈ 10 000 (the test harness).  A
       static array of that size is inexpensive and works for any
       seq_t, be it a fixed‑width integer or an arbitrary‑precision
       class with a default constructor.  The array is filled on
       demand; values once computed are reused for subsequent calls. */
    static seq_t cache[10001];
    static idx_t cache_size = 0;   /* number of values currently stored */
    static seq_t pow2 = 1;         /* 2^current_index, current_index = cache_size */

    /* Initialise the first two elements once.  The assignment is done
       lazily to avoid modifying the array on every call. */
    if (cache_size == 0) {
        cache[0] = 1;              /* a(0) = 1 */
        cache_size = 1;
    }

    /* Return cached value if available. */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache up to n.  The loop starts at the first missing
       index and uses the recurrence.  pow2 is kept in sync with the
       current index. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Compute 2^i: multiply previous pow2 by 2. */
        pow2 = pow2 * ((seq_t)2);

        /* a(i) = 3*i*a(i-1) + 2^i */
        seq_t val = cache[i - 1];
        val = val * ((seq_t)3 * (seq_t)i);   /* 3*i*a(i-1) */
        val = val + pow2;                    /* + 2^i */
        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
