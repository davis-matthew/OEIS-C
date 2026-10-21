/*
 * A213923  Minimal lengths of formulas representing n only using addition,
 *          multiplication and the constant 1.
 * https://oeis.org/A213923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A005245(n)-1 (A005245 gives the minimal number of 1's
 *            needed to build n using addition, multiplication, and the
 *            constant 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* cache[i] holds the value of A005245(i);  cache[0] is unused. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;        /* largest n for which cache[n] is valid */

/* Extend cache to include all values up to max_n (inclusive).  */
static void extend_cache(idx_t max_n)
{
    if (max_n <= cache_cap)
        return;

    /* Reallocate array to new size.  */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(max_n + 1) * sizeof(seq_t));
    if (!new_cache) {
        abort();               /* Allocation failure – terminate.   */
    }
    cache = new_cache;

    /* First allocation: set base cases.  */
    if (cache_cap == 0) {
        cache[0] = 0;          /* unused but harmless. */
        cache[1] = 1;          /* A005245(1) = 1 */
    }

    /* Compute A005245 values progressively.  */
    for (idx_t n = cache_cap + 1; n <= max_n; ++n) {
        /* Lower bound: n = 1 + (n‑1).  */
        seq_t best = cache[1] + cache[n - 1];

        /* Additive splits:  n = i + (n‑i)  with 1 ≤ i ≤ n/2.  */
        for (idx_t i = 1; i <= n / 2; ++i) {
            seq_t v = cache[i] + cache[n - i];
            if (v < best)
                best = v;
        }

        /* Multiplicative splits:  n = i * j,  i ≥ 2, j ≥ 2.  */
        for (idx_t i = 2; (idx_t)i * i <= n; ++i) {
            if (n % i == 0) {
                idx_t j = n / i;
                if (j >= 2) {
                    seq_t v = cache[i] + cache[j];
                    if (v < best)
                        best = v;
                }
            }
        }
        cache[n] = best;
    }
    cache_cap = max_n;
}

/* Public sequence function.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* Undefined for n < 1 – safe default. */

    extend_cache(n);

    /* a(n) = 2 * A005245(n) - 1.  All arithmetic stays in seq_t.  */
    return (seq_t)2 * cache[n] - (seq_t)1;
}
