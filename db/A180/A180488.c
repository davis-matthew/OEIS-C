/*
 * A180488  Partial sums of A006864.
 * https://oeis.org/A180488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-3) + 3*a(n-4) - a(n-5) Initial values:
 *            a(1)=0, a(2)=1, a(3)=3, a(4)=9, a(5)=23 The implementation
 *            memoises values up to the largest n requested. For builds where
 *            seq_t is an unsigned integer of any size or an
 *            arbitrary‑precision integer class, the arithmetic above is valid
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t cache[10001];          /* large enough for n ≈ 10000 */
static idx_t computed = 0;          /* number of terms stored (computed == 0: uninitialised) */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for indices ≤ 0, return 0 for safety */

    /* Lazy initialisation of the first 5 values */
    if (computed == 0) {
        cache[0] = (seq_t)0;     /* a(1) */
        cache[1] = (seq_t)1;     /* a(2) */
        cache[2] = (seq_t)3;     /* a(3) */
        cache[3] = (seq_t)9;     /* a(4) */
        cache[4] = (seq_t)23;    /* a(5) */
        computed = 5;
    }

    if (n <= computed)
        return cache[n - 1];

    /* Compute missing terms up to n */
    for (idx_t i = computed; i <= n - 1; ++i) {
        /* Compute left = 3*a[i-1] + 3*a[i-4]    (indices 0‑based) */
        seq_t left  = cache[i - 1] * (seq_t)3 + cache[i - 4] * (seq_t)3;
        /* Compute right = 4*a[i-3] + a[i-5] */
        seq_t right = cache[i - 3] * (seq_t)4 + cache[i - 5];
        cache[i] = left - right;           /* final recurrence */
    }

    computed = n;
    return cache[n - 1];
}
