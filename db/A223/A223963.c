/*
 * A223963  Number of 3 X n 0..3 arrays with antidiagonals unimodal and rows
 *          and diagonals nondecreasing.
 * https://oeis.org/A223963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n‑1) - 45*a(n‑2) + 120*a(n‑3) - 210*a(n‑4) +
 *            252*a(n‑5) - 210*a(n‑6) + 120*a(n‑7) - 45*a(n‑8) + 10*a(n‑9) -
 *            a(n‑10) initial values: a(1)=64, a(2)=707, a(3)=3471,
 *            a(4)=12311, a(5)=36028, a(6)=92734, a(7)=217144, a(8)=471994,
 *            a(9)=965172, a(10)=1874532, a(11)=3482781, a(12)=6225291
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 28, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;    /* array indexed from 1, index 0 unused */
static size_t cache_cap = 0;   /* current allocated capacity (≥ cache_len+1) */
static idx_t cache_len = 0;    /* highest index that has been computed */

static void
ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;                /* already have the value */

    /* Ensure the allocated array is big enough */
    size_t needed = (size_t)n + 1;          /* index 0 unused, so need n+1 slots */
    if (cache_cap < needed)
        grow_seq_array(&cache, &cache_cap, needed);

    /* Initialise the first 12 terms only once */
    if (cache_len == 0) {
        cache[1]   = (seq_t)64;
        cache[2]   = (seq_t)707;
        cache[3]   = (seq_t)3471;
        cache[4]   = (seq_t)12311;
        cache[5]   = (seq_t)36028;
        cache[6]   = (seq_t)92734;
        cache[7]   = (seq_t)217144;
        cache[8]   = (seq_t)471994;
        cache[9]   = (seq_t)965172;
        cache[10]  = (seq_t)1874532;
        cache[11]  = (seq_t)3482781;
        cache[12]  = (seq_t)6225291;
        cache_len = 12;
    }

    /* Compute any remaining terms up to n using the recurrence above */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = (seq_t)10   * cache[i - 1]
                  - (seq_t)45   * cache[i - 2]
                  + (seq_t)120  * cache[i - 3]
                  - (seq_t)210  * cache[i - 4]
                  + (seq_t)252  * cache[i - 5]
                  - (seq_t)210  * cache[i - 6]
                  + (seq_t)120  * cache[i - 7]
                  - (seq_t)45   * cache[i - 8]
                  + (seq_t)10   * cache[i - 9]
                  - cache[i -10];
        cache[i] = val;
    }

    if (cache_len < n)
        cache_len = n;
}

seq_t
a(idx_t n)
{
    if (n == 0)   /* undefined for the sequence, but return 0 safely */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
