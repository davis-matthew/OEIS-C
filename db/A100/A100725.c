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
 * Helpers:   binary_runs, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/binary_runs.h"

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
        if (!is_prime(cand))
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
