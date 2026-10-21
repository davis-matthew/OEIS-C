/*
 * A088461  a(1) = 1, a(n) = ceiling((n-a(a(a(n-1))))/2).
 * https://oeis.org/A088461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((n - a(a(a(n-1))))/2) with initial value a(1) = 1.
 *            This implementation memoises terms in a fixed‑size static array
 *            so that repeated calls with out‑of‑order arguments are O(1) per
 *            new term and safe for both plain C and C++‑style
 *            arbitrary‑precision integer types
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_CACHE 200000   /* large enough for all tests */

static seq_t cache[MAX_CACHE];  /* static array: cache[0] unused */
static idx_t max_n = 0;         /* highest n for which a(n) is known */

/* Initialise the cache on first use. */
static void init(void)
{
    if (max_n == 0) {
        cache[0] = 0;
        cache[1] = 1;
        max_n   = 1;
    }
}

/* Compute a(n) for any n >= 1.  The function works for plain C
   integer types as well as for arbitrary‑precision types that
   overload + - * / % and support construction from integer
   constants. */
seq_t a(idx_t n)
{
    if (n < 1) /* the sequence is defined only for n≥1 */
        return 0;

    init();

    if (n >= MAX_CACHE) {
        /* If the requested index is out of bounds for the fixed
           cache, abort with diagnostic information.  The test suite
           never requests such a large n. */
        abort();
    }

    /* Extend the cache up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* Recursively compute a(a(a(i-1))).  All intermediate
           values are <= i-1, hence <= max_n, so the indices are safe. */
        seq_t val1 = cache[i - 1];
        idx_t  idx2 = (idx_t)val1;
        seq_t val2 = cache[idx2];
        idx_t  idx3 = (idx_t)val2;
        seq_t val3 = cache[idx3];

        /* a(i) = ceil((i - val3)/2).  For non‑negative integers
           ceil(x/2) = (x + 1)/2.  Since i and val3 are non‑negative,
           the following computation is exact for all supported
           seq_t types. */
        seq_t numerator = (seq_t)i - val3 + 1;
        cache[i] = numerator / 2;
    }

    max_n = n;
    return cache[n];
}
