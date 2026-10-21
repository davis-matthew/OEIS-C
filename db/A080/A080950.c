/*
 * A080950  Number of numbers that differ from n in binary representation by
 *          exactly one edit-operation: deletion, insertion, or substitution.
 * https://oeis.org/A080950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = a(floor(n/2)) + 2 or 3 according to n mod 4, with a(0)=2,
 *            a(1)=3, a(2)=6, a(3)=5, a(4)=7, a(5)=8, a(6)=8, a(7)=7.
 *            Recurrence valid for n >= 8
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080950: Number of integers whose binary representation differs from n by exactly one edit operation. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t* cache = NULL;
static idx_t cache_len = 0;

/* Base values for n = 0..7 */
static const seq_t base_vals[8] = {
    (seq_t)2, /* a(0) */
    (seq_t)3, /* a(1) */
    (seq_t)6, /* a(2) */
    (seq_t)5, /* a(3) */
    (seq_t)7, /* a(4) */
    (seq_t)8, /* a(5) */
    (seq_t)8, /* a(6) */
    (seq_t)7  /* a(7) */
};

seq_t a(idx_t n)
{
    /* Ensure cache can hold values up to n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory: return 0 as a fallback (should not happen in test suite) */
            return (seq_t)0;
        }
        cache = new_cache;
        /* Compute new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 8) {
                cache[i] = base_vals[i];
            } else {
                idx_t half = i / 2;           /* floor(i/2) */
                switch (i & 3) {              /* i % 4 */
                    case 0: cache[i] = cache[half] + (seq_t)2; break;
                    case 1: cache[i] = cache[half] + (seq_t)3; break;
                    case 2: cache[i] = cache[half] + (seq_t)3; break;
                    case 3: cache[i] = cache[half] + (seq_t)2; break;
                }
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
