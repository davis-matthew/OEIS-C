/*
 * A187813  Numbers n whose base-b digit sum is not b for all bases b >= 2.
 * https://oeis.org/A187813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 6508, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187813:
   a(n) is the n-th non‑negative integer such that for every base b ≥ 2,
   the sum of its digits in base b is not equal to b.  Offset: 1 (a(1)=0). */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the sum of the digits of value when represented in the given base. */
static seq_t digit_sum_base(seq_t value, seq_t base)
{
    seq_t sum = 0;
    while (value) {
        sum += value % base;
        value /= base;
    }
    return sum;
}

/* Return true iff the value satisfies the “digit sum ≠ base” property for
   all bases b ≥ 2.  The test uses the fact that b–1 divides value–1. */
static bool passes(seq_t value)
{
    /* 0 and 1 always satisfy the property. */
    if (value <= (seq_t)1) return true;

    seq_t n_minus_1 = value - 1;
    /* Iterate over all positive divisors d of value‑1. */
    for (idx_t d = 1; (seq_t)d * (seq_t)d <= n_minus_1; d++)
    {
        if (n_minus_1 % (seq_t)d == 0)
        {
            seq_t base1 = (seq_t)d + 1;
            if (digit_sum_base(value, base1) == base1) return false;

            idx_t d2 = (idx_t)(n_minus_1 / (seq_t)d);
            if (d2 != d) {
                seq_t base2 = (seq_t)d2 + 1;
                if (digit_sum_base(value, base2) == base2) return false;
            }
        }
    }
    return true;
}

/* Emit the n-th term of the sequence.  The function keeps a cache of
   already computed terms and generates new ones as required. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* cached terms */
    static size_t cache_size = 0;     /* number of terms cached */
    static size_t cache_cap = 0;      /* allocated capacity of cache */
    static idx_t next_candidate = 0;  /* next integer to test */

    if (n == 0) return 0;            /* undefined behavior for offset <1 */

    /* Grow the cache until we have at least n terms. */
    while (cache_size < n) {
        /* Ensure there is room for one more term. */
        if (cache_size + 1 > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        seq_t cand = (seq_t)next_candidate;
        next_candidate++;            /* advance for next time */

        if (passes(cand)) {
            cache[cache_size++] = cand;
        }
    }
    return cache[n - 1];
}
