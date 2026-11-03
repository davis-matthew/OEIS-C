/*
 * A227187  Numbers n whose factorial base representation A007623(n) contains
 *          at least one nonleading zero. (Zero is also included as a(0)).
 * https://oeis.org/A227187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0, a(1)=2; for n>1, * let z = number of zero digits in the
 *            factorial‑base representation * of a(n‑1) (excluding the most
 *            significant digit). If a(n‑1) is odd * or z>1, then
 *            a(n)=a(n‑1)+1, otherwise a(n)=a(n‑1)+2. * The function memoises
 *            results so that calls with arbitrary n * are fast and c...
 * Range:     u8 n <= 198, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9674 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static int count_nonleading_zero_digits(seq_t x)
{
    /* 0 and 1 do not have any non‑leading zero in their factorial base
       representations, so we return 0 for them. */
    if (x < 2)
        return 0;

    /* Find the largest k such that k! <= x.  We build factorials
       incrementally while avoiding overflow. */
    seq_t fact = 1;     /* currently k!   (k starts as 1) */
    size_t k = 1;       /* k! corresponds to factorial of k */

    while (1) {
        /* If (k+1)! would exceed x (or would overflow), stop. */
        if (fact > x / (k + 1))
            break;
        fact *= (k + 1);
        ++k;
    }

    /* Now fact == k!  and k! <= x < (k+1)!.  Count zero digits
       among positions 1..k (i.e., coefficients for 1!,2!,…k!). */
    seq_t rem = x;
    int zeros = 0;
    for (size_t i = k; ; --i) {
        seq_t digit = rem / fact;
        if (digit == 0)
            ++zeros;
        rem -= digit * fact;
        if (i == 1)      /* finished the 1! position */
            break;
        fact /= (seq_t)i;   /* move to (i‑1)! for next iteration */
    }

    return zeros;
}

seq_t a(idx_t n)
{
    /* Static cache of computed sequence values. */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* allocated number of slots */
    static idx_t size = 0;       /* number of terms stored: cache[0..size-1] */

    /* Return cached value if already available. */
    if (n < size)
        return cache[n];

    /* Expand the cache to accommodate indices up to n. */
    if (n + 1 > capacity) {
        capacity = n + 1;
        cache = mem_resize(seq_t, cache, capacity);
    }

    /* Compute missing terms in order. */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0)
            cache[i] = 0;
        else if (i == 1)
            cache[i] = 2;
        else {
            seq_t prev   = cache[i - 1];
            int zeros   = count_nonleading_zero_digits(prev);
            if ((prev & 1) != 0 || zeros > 1)
                cache[i] = prev + 1;
            else
                cache[i] = prev + 2;
        }
    }

    size = n + 1;
    return cache[n];
}
