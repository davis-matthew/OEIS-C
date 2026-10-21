/*
 * A240845  Numbers m equal to the product of all of their integral proper
 *          roots (defined as numbers k in the range 1 < k < m such that k^j =
 *          m for some j).
 * https://oeis.org/A240845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A240845 – Numbers equal to the product of all of their integral proper roots.
   Formula:  a(n) = n‑th smallest integer of the form a^p where
   a is not a perfect power and p is a perfect number.  The first two
   perfect numbers are 6 and 28, which generate all terms that fit into
   64‑bit integers for n <= 10 000.  A simple multi‑sequence merge is used. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_pow.h"

/* Return true iff n (>1) is a perfect power: n = b^e for some e >= 2. */
static bool is_perfect_power(uint64_t n)
{
    if (n < 2) return false;
    for (uint64_t base = 2; base * base <= n; ++base) {
        uint64_t p = base * base;
        while (p < n) {          /* avoid overflow: p < n guarantees safe multiply */
            p *= base;
            if (p == n) return true;
        }
        if (p == n) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined call, safe fallback */

    /* Perfect numbers to use – only the first two are needed for n up to ~10 000. */
    static const idx_t exps[2] = {6ULL, 28ULL};
    size_t i;

    uint64_t bases[2];
    seq_t  cur[2];

    /* Initialise current bases and values. */
    for (i = 0; i < 2; ++i) {
        bases[i] = 2;
        while (is_perfect_power(bases[i])) ++bases[i];
        cur[i] = seq_pow((seq_t)bases[i], exps[i]);
    }

    seq_t result = 0;

    for (idx_t cnt = 1; cnt <= n; ++cnt) {
        /* Find the smallest current value. */
        size_t min_idx = 0;
        for (i = 1; i < 2; ++i)
            if (cur[i] < cur[min_idx]) min_idx = i;

        result = cur[min_idx];

        /* Advance the chosen sequence. */
        do {
            ++bases[min_idx];
        } while (is_perfect_power(bases[min_idx]));
        cur[min_idx] = seq_pow((seq_t)bases[min_idx], exps[min_idx]);
    }

    return result;
}
