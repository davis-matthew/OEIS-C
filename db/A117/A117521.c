/*
 * A117521  Start with 1 and repeatedly reverse the digits and add 2 to get
 *          the next term.
 * https://oeis.org/A117521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = reverse_decimal(a(n-1)) + 2
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

static seq_t cache[10001];
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* unused, but safe */

    if (cache_size == 0) {                /* initialise with a(1) */
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    if (n <= cache_size)                  /* already computed */
        return cache[n - 1];

    /* compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t prev = cache[cache_size - 1];
        seq_t next = reverse_decimal(prev) + (seq_t)2;
        cache[cache_size] = next;
        ++cache_size;
    }

    return cache[n - 1];
}
