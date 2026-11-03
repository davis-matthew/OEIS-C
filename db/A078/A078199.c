/*
 * A078199  Smallest multiple of n (for n >= 100) in which frequency of each
 *          occurring digit is the same.
 * https://oeis.org/A078199
 *
 * Offset:    100    (a(100) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k≥1 such that (k·n) has the same frequency for
 *            every decimal digit that actually appears in k·n. The
 *            implementation simply searches multiples of n until the
 *            condition is met
 * Range:     u8 none, u16 n <= 999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9068 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the smallest multiple of n satisfying the equal‑frequency
   property. */
static seq_t find_min_multiple(idx_t n)
{
    seq_t inc = (seq_t)n;   /* step size */
    seq_t val = inc;        /* current multiple (k = 1) */

    while (true) {
        unsigned freq[10] = {0};
        seq_t tmp = val;

        /* Count digits of val */
        while (tmp > 0) {
            int d = (int)(tmp % 10);
            freq[d]++;
            tmp /= 10;
        }

        /* Verify that all non‑zero frequencies are equal */
        unsigned target = 0;
        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if (freq[d] > 0) {
                if (target == 0)
                    target = freq[d];
                else if (freq[d] != target) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok)
            return val;          /* found the smallest multiple */

        val += inc;             /* next multiple */
    }
}

/* External function required by the OEIS framework.  For indices below
   the offset (100) the behavior is irrelevant, so we simply return 0. */
seq_t a(idx_t n)
{
    if (n < 100)          /* offset: n = 100 is the first valid term */
        return (seq_t)0;
    return find_min_multiple(n);
}
