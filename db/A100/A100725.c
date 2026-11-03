/*
 * A100725  Prime numbers whose binary representations are split into a
 *          maximum of 5 runs.
 * https://oeis.org/A100725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th prime whose binary representation has at most 5
 *            runs of consecutive equal bits (OEIS A100725)
 * Range:     u8 n <= 51, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

/* Count the number of runs of consecutive identical bits in x.
   An early exit returns if more than 5 runs are found. */
static inline int binary_runs(uint64_t x)
{
    if (x == 0) return 1;          /* binary 0 has one run of 0s */

    int runs  = 1;
    uint64_t prev = x & 1ULL;
    x >>= 1;
    while (x)
    {
        uint64_t cur = x & 1ULL;
        if (cur != prev)
        {
            ++runs;
            if (runs > 5) return runs;  /* early exit */
        }
        prev = cur;
        x >>= 1;
    }
    return runs;
}

#define MAX_GOOD 20000  /* enough for n up to ~10 000 */

static seq_t   good[MAX_GOOD];
static idx_t   good_cnt   = 0;
static uint64_t last_candidate = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, harmless */

    while (good_cnt < n)
    {
        uint64_t cand = ++last_candidate;

        /* test primality */
        if (!is_prime_u64(cand))
            continue;              /* not prime */

        /* test binary runs */
        if (binary_runs(cand) <= 5)
        {
            if (good_cnt < MAX_GOOD)
                good[good_cnt] = (seq_t)cand;   /* store the prime */
            ++good_cnt;
        }
    }
    return good[n - 1];
}
