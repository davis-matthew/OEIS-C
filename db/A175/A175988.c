/*
 * A175988  a(1) = 1, for n >= 2: a(n) = (a(n-1) - 1)^n.
 * https://oeis.org/A175988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n >= 2: a(n) = (a(n-1)-1)^n
 * Range:     i8 n <= 4, i16 n <= 4, i32 n <= 5, i64 n <= 5, i128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* fast exponentiation for seq_t values */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U) result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}

/* Cache up to index 10000.  The sequence grows rapidly, so 10000
   suffices for all tests.  For n > 10000 we compute on the fly and
   do not store the terms. */
static seq_t cache[10001];
static idx_t cached_max = 0;   /* highest index already cached */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* out of range; offset is 1 */

    /* Return cached value if it already exists */
    if (n <= cached_max) return cache[n];

    /* Ensure a(1) is cached */
    if (cached_max == 0)
    {
        cache[1] = (seq_t)1;
        cached_max = 1;
    }

    /* Cache values up to min(n, 10000) */
    idx_t up_to = n;
    if (up_to > 10000) up_to = 10000;
    for (idx_t i = cached_max + 1; i <= up_to; ++i)
    {
        seq_t base = cache[i - 1] - (seq_t)1;
        cache[i] = pow_seq(base, i);
    }
    cached_max = up_to;

    if (n <= 10000) return cache[n];

    /* For indices > 10000 compute without caching */
    seq_t val = cache[10000];
    for (idx_t i = 10001; i <= n; ++i)
    {
        seq_t base = val - (seq_t)1;
        val = pow_seq(base, i);
    }
    return val;
}
