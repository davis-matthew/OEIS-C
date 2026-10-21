/*
 * A219729  Sum_{x <= n} largest divisor of x that is <= sqrt(x).
 * https://oeis.org/A219729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{x=1..n} largest divisor d of x such that d <=
 *            sqrt(x)
 * Range:     u8 n <= 78, u16 n <= 3814, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

/* Cache of cumulative sums a(1)..a(max_n) */
static seq_t *prefix = NULL;     /* prefix[i] = a(i), prefix[0] unused */
static idx_t cache_size = 0;      /* highest n currently cached */

/* Compute largest divisor of x that is <= sqrt(x) */
static seq_t largest_divisor_le_sqrt(idx_t x)
{
    idx_t d = _floor_isqrt_u64(x);   /* floor(sqrt(x)) */
    for (; d > 0; --d) {
        if (x % d == 0) {
            return (seq_t)d;
        }
    }
    return (seq_t)1;    /* unreachable for x >= 1 */
}

/* Ensure prefix[1..n] is computed.  Expands cache as needed. */
static void ensure_prefix(idx_t n)
{
    if (n <= cache_size) return;

    idx_t new_cap = (cache_size == 0) ? 1 : cache_size;
    while (new_cap < n) new_cap <<= 1;

    seq_t *new_pref = (seq_t *)realloc(prefix, (new_cap + 1) * sizeof(seq_t));
    if (!new_pref) abort();            /* allocation failed */
    prefix = new_pref;

    /* initialize prefix[0] if first allocation */
    if (cache_size == 0) {
        prefix[0] = (seq_t)0;
    }

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t f = largest_divisor_le_sqrt(i);
        prefix[i] = prefix[i - 1] + f;
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, but safe */
    ensure_prefix(n);
    return prefix[n];
}
