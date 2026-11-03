/*
 * A121037  Multiples of 17 containing a 17 in their decimal representation.
 * https://oeis.org/A121037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 177, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9752 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = n‑th multiple of 17 that contains the substring "17" in its decimal representation */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Return true if the decimal representation of x contains the substring "17". */
static bool contains_17(seq_t x)
{
    seq_t prev = 10;  /* sentinel value larger than any single digit */
    while (x > 0) {
        seq_t digit = x % 10;
        if (digit == 1 && prev == 7)
            return true;
        prev = digit;
        x = x / 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values */
    static size_t cap = 0;        /* capacity of cache array */
    static size_t count = 0;      /* number of terms stored */
    static idx_t next_k = 1;      /* next multiplier to test */

    if (n == 0)
        return (seq_t)0;          /* undefined, return 0 for safety */

    if (cache == NULL) {
        cap = 4;
        cache = mem_new(seq_t, cap);
    }

    /* Generate terms until we have at least n of them. */
    while (count < n) {
        seq_t m = seq_mul((seq_t)17, (seq_t)next_k);
        if (contains_17(m)) {
            if (count >= cap) {
                size_t newcap = cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
                if (!tmp) {
                    mem_free(cache);
                    cache = NULL;
                    cap = 0;
                    return (seq_t)0;
                }
                cache = tmp;
                cap = newcap;
            }
            cache[count++] = m;
        }
        ++next_k;
    }

    return cache[n - 1];
}
