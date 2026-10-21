/*
 * A063269  a(1) = 3, a(n) = concatenation of two closest factors of a(n-1)
 *          whose product equals a(n-1) or if a(n-1) is a prime then the
 *          concatenation of 1 and a(n-1).
 * https://oeis.org/A063269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   digit_len, ensure_cache, floor_sqrt, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3; for n>1, let x=a(n‑1).  
   If x is prime, a(n)=concat(1,x); otherwise let d be the largest
   divisor of x not exceeding sqrt(x) and m=x/d, then
   a(n)=concat(d,m). */

/* External integer types are defined by the build (seq_t, idx_t). */
#include <stdbool.h>
#include <stddef.h>

#include "../utils/ensure_cache.h"
#include "../utils/floor_sqrt.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 1, but return 0 as a safe default. */
    if (n < 1) return 0;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;      /* array of a(k) for k=1..cap */
    static size_t  cap   = 0;        /* current capacity of cache */
    static size_t  last  = 0;        /* number of terms already computed */

    /* Make sure the cache is large enough to hold n terms. */
    if (n > cap)
        ensure_cache(n, &cache, &cap);

    /* If the term is already known, just return it. */
    if (n <= last)
        return cache[n - 1];

    /* Compute missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t value;
        if (i == 1) {
            value = 3;                      /* a(1) = 3 */
        } else {
            seq_t prev = cache[i - 2];     /* a(i‑1) */

            if (is_prime_big(prev)) {
                /* Concatenate 1 and prev. */
                seq_t pow = pow10((idx_t)digit_len(prev));
                value = (seq_t)1 * pow + prev;
            } else {
                /* Find the closest factor pair (d,m). */
                seq_t d = floor_sqrt(prev);
                seq_t m = 0;
                for (; d >= 2; --d) {
                    if ((prev % d) == 0) {
                        m = prev / d;
                        break;
                    }
                }
                /* Concatenate d and m. */
                seq_t pow = pow10((idx_t)digit_len(m));
                value = d * pow + m;
            }
        }
        cache[i - 1] = value;   /* store a(i) */
    }

    last = n;
    return cache[n - 1];
}
