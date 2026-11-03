/*
 * A088720  Unique monotone sequence satisfying a(a(a(n))) = 2n.
 * https://oeis.org/A088720
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is defined for n ≥ 3 by the recurrence * * a(2n) = 2 * a(n)
 *            * a(4n+1) = a(2n+1) + 2 * a(n) * a(4n+3) = 3 * a(2n+1) – 2 *
 *            a(n) * * Base values are * a(3)=4, a(4)=5, a(5)=6, a(6)=8,
 *            a(7)=9, a(8)=10, * a(9)=11, a(10)=12, a(11)=14 * * The function
 *            below memoises these values so that any * call t...
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9589 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Recursive helper that implements the recurrence. */
static seq_t
compute_a(idx_t n)
{
    /* Base cases from the OEIS entry. */
    switch (n) {
        case 3: return 4;
        case 4: return 5;
        case 5: return 6;
        case 6: return 8;
        case 7: return 9;
        case 8: return 10;
        case 9: return 11;
        case 10: return 12;
        case 11: return 14;
    }

    /* For n ≥ 12 apply the recurrence. */
    if ((n & 1) == 0) {                 /* even: a(2k) = 2*a(k) */
        idx_t k = n >> 1;
        return 2 * a(k);
    }
    if ((n & 3) == 1) {                 /* n ≡ 1 (mod 4): a(4k+1) */
        idx_t k = (n - 1) >> 2;
        seq_t a_2k1 = a(2 * k + 1);
        seq_t a_k   = a(k);
        return a_2k1 + 2 * a_k;
    }
    /* n ≡ 3 (mod 4): a(4k+3) */
    idx_t k = (n - 3) >> 2;
    seq_t a_2k1 = a(2 * k + 1);
    seq_t a_k   = a(k);
    return 3 * a_2k1 - 2 * a_k;
}

static seq_t *cache = NULL;     /* memoisation array */
static idx_t cache_size = 0;    /* number of allocated slots */

/* Ensure that cache can hold indices up to n. */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    /* Grow the cache exponentially for amortised linear time. */
    idx_t new_size = cache_size ? cache_size * 2 : 16;
    while (new_size <= n)
        new_size <<= 1;

    seq_t *tmp = mem_resize(seq_t, cache, (size_t)new_size);
    if (!tmp) {
        /* Allocation failed – the caller will crash or return 0. */
        return;
    }
    /* Initialise newly allocated slots to 0. */
    for (idx_t i = cache_size; i < new_size; ++i)
        tmp[i] = 0;
    cache = tmp;
    cache_size = new_size;
}

/* Forward declaration of the recursive helper. */
static seq_t compute_a(idx_t n);

/* Public entry point.  For n < 3 the value is unspecified; we return 0. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    ensure_cache(n);
    if (cache[n] != 0)
        return cache[n];

    cache[n] = compute_a(n);
    return cache[n];
}
