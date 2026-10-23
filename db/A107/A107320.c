/*
 * A107320  a(1)=1 then a(n)=f(n)-a(f(n))+a(f(n)-a(f(n))) where
 *          f(n)=floor(n/2).
 * https://oeis.org/A107320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, for n ≥ 2: * f = floor(n/2) * a(n) = f - a(f) + a( f -
 *            a(f) ) * (We define a(0)=1 to support the recursion for n=2,3.)
 *            * This implementation memoises the values in a static array and
 *            * is portable to C11 and C++17
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>  /* for uint64_t used by some utilities */

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *dp = NULL;     /* dp[i] holds a(i) for i ≥ 0          */
    static size_t cap = 0;       /* current capacity of dp             */

    /* Handle the base case for recursion (a(0) = 1) */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Initialise the table on first use */
    if (cap == 0) {
        /* allocate room for indices 0 and 1 */
        size_t init_cap = 2;
        dp = (seq_t*)realloc(NULL, init_cap * sizeof(seq_t));
        if (!dp) abort();                     /* allocation failure */
        dp[0] = dp[1] = (seq_t)1;            /* a(0) = a(1) = 1 */
        cap  = init_cap;
    }

    /* Grow the table if the requested index is out of range */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) abort();                   /* allocation failure */
        /* initialise new entries to 0 (uncomputed) */
        for (size_t i = cap; i < new_cap; ++i) {
            tmp[i] = (seq_t)0;
        }
        dp  = tmp;
        cap = new_cap;
    }

    /* Return the cached value if already computed */
    if (dp[n] != (seq_t)0) {
        return dp[n];
    }

    /* Recursive computation of a(n) */
    idx_t f   = n / 2;
    seq_t af  = a(f);                            /* a(f)   */
    /* f - a(f) is non‑negative; cast af to idx_t safely
       because f ≥ a(f) for all n in the sequence. */
    idx_t diff = (f >= (idx_t)af) ? (f - (idx_t)af) : 0;
    seq_t adiff = a(diff);                       /* a(f-a(f)) */

    seq_t result = (seq_t)f - af + adiff;
    dp[n] = result;
    return result;
}
