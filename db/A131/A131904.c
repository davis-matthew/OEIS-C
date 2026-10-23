/*
 * A131904  Smallest positive integer k with the same number of divisors as
 *          the n-th integer for which such a k exists.
 * https://oeis.org/A131904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A131904:
 * a(n) is the smallest integer k having the same number of divisors
 * as the n‑th integer that is NOT the minimal integer for its divisor
 * count.  We generate integers m = 1,2,3,…, keep the first (minimal)
 * integer for each divisor count d in an array min_for_d[d].  When we
 * encounter a new integer m with a divisor count that we have seen
 * before, m is not minimal; we output the stored minimal integer
 * min_for_d[d] as the next term of the sequence.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Helper: divisor count for a 64‑bit integer                            */
/* ------------------------------------------------------------------ */
static seq_t divisor_count_u64(uint64_t n)
{
    seq_t count = 1;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            count *= (seq_t)(exp + 1);
        }
    }
    if (tmp > 1)                /* remaining prime factor */
        count *= 2;
    return count;
}

/* ------------------------------------------------------------------ */
/* Global state for memoization                                         */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;       /* sequence a(n) values               */
static size_t seq_len  = 0;           /* how many terms already computed    */
static size_t seq_cap  = 0;           /* capacity of seq_cache              */

static seq_t *min_for_d = NULL;       /* minimal integer for each divisor count */
static size_t min_cap    = 0;         /* capacity of min_for_d               */

static idx_t next_m = 1;              /* next integer to examine             */

/* Ensure the sequence cache holds at least target terms.            */
static void ensure_cache_size(size_t target)
{
    if (seq_len >= target)
        return;

    /* initialize min_for_d table if needed */
    if (min_for_d == NULL) {
        min_cap   = 1024;
        min_for_d = (seq_t *)calloc(min_cap, sizeof(seq_t));
        if (!min_for_d)
            exit(EXIT_FAILURE);
    }

    /* grow the sequence cache as needed */
    if (seq_cap < target) {
        seq_cap = target < 1024 ? 1024 : target;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        if (!seq_cache)
            exit(EXIT_FAILURE);
    }

    while (seq_len < target) {
        ++next_m;                         /* examine next integer       */
        seq_t d = divisor_count_u64((uint64_t)next_m);
        size_t idx = (size_t)d;

        /* enlarge min_for_d table if divisor count too large          */
        if (idx >= min_cap) {
            size_t old_cap = min_cap;
            min_cap = idx + 1024;
            seq_t *tmp = (seq_t *)realloc(min_for_d, min_cap * sizeof(seq_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            min_for_d = tmp;
            for (size_t k = old_cap; k < min_cap; ++k)
                min_for_d[k] = 0;
        }

        /* record the first (minimal) integer with this divisor count */
        if (min_for_d[idx] == 0)
            min_for_d[idx] = (seq_t)next_m;
        else if (min_for_d[idx] < (seq_t)next_m) {
            /* this integer is not minimal; output its minimal counterpart */
            seq_cache[seq_len++] = min_for_d[idx];
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined, but return 0 for safety */

    ensure_cache_size((size_t)n);
    return seq_cache[(size_t)n - 1];
}
