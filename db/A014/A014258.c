/*
 * A014258  Iccanobif numbers: add previous two terms and reverse the sum,
 *          starting with 0, 1.
 * https://oeis.org/A014258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 37, u64 n <= 64, u128 n <= 124,
 *            big any n
 * Helpers:   ensure_cache, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n)=reverse_decimal(a(n-1)+a(n-2)) for n>=2 */

#include "../utils/ensure_cache.h"
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;
    static size_t cap      = 0;
    static idx_t computed  = 0;           // number of terms stored (indices 0 .. computed-1)

    if (n < 2)                             // a(0)=0, a(1)=1
        return n;

    if (computed == 0) {                   // initialise cache for base cases
        ensure_cache(1, &cache, &cap);     // need space for indices 0 and 1
        cache[0] = 0;
        cache[1] = 1;
        computed = 2;
    }

    if (n + 1 > cap)
        ensure_cache(n, &cache, &cap);     // grow cache if needed

    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        cache[i] = reverse_decimal(sum);
    }

    computed = n + 1;
    return cache[n];
}
