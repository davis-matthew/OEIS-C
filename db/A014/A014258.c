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
 * Validated: standard, pass, 53 terms through n = 963 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n)=reverse_decimal(a(n-1)+a(n-2)) for n>=2 */

#include <stdlib.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

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
        cache[i] = reverse_digits(sum);
    }

    computed = n + 1;
    return cache[n];
}
