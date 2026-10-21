/*
 * A080943  Numbers having exactly two divisors that are also suffixes in
 *          binary representation.
 * https://oeis.org/A080943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer having exactly two divisors that
 *            are also suffixes of its binary representation
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/bitlen_seq_t.h"

/* Determine whether the integer x has exactly two distinct divisors that
   are suffixes of its binary representation. */
static inline bool has_two_suffix_divisors(seq_t x)
{
    idx_t L = (idx_t)bitlen_seq_t(x);   /* number of bits in x */
    idx_t cnt = 0;
    seq_t seen[128];                     /* at most 128 suffix lengths */

    for (idx_t len = 1; len <= L; ++len) {
        seq_t d;
        if (len == L)
            d = x;                       /* full length suffix equals x */
        else {
            seq_t mask = (((seq_t)1) << len) - (seq_t)1;
            d = x & mask;
        }

        if (d == (seq_t)0)
            continue;                    /* zero cannot be a divisor */

        bool already = false;
        for (idx_t i = 0; i < cnt; ++i)
            if (seen[i] == d) { already = true; break; }
        if (already)
            continue;

        if ((x % d) == (seq_t)0) {
            seen[cnt++] = d;
            if (cnt > 2)
                return false;            /* more than two suffix divisors */
        }
    }
    return cnt == 2;
}

/* Return the n-th element of the OEIS sequence A080943. */
seq_t a(idx_t n)
{
    static idx_t found = 0;              /* number of terms found so far */
    static seq_t *cache = NULL;          /* array of found terms */
    static size_t cache_cap = 0;         /* capacity of cache */
    static seq_t current = (seq_t)1;     /* next integer to test */

    /* Extend cache until we have at least n terms. */
    while (found < n) {
        if (has_two_suffix_divisors(current)) {
            ++found;
            ensure_capacity((size_t)found, &cache_cap, (void **)&cache,
                            sizeof(seq_t));
            cache[found - 1] = current;
        }
        ++current;
    }

    return cache[n - 1];
}
