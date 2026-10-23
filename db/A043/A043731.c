/*
 * A043731  Numbers whose number of runs in their base-2 representation is
 *          congruent to 3 mod 5.
 * https://oeis.org/A043731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: iterate positive integers, keep those whose binary run count
   (number of maximal runs of equal bits in the base‑2 representation)
   satisfies run_count ≡ 3 (mod 5).  a(n) is the nth such integer. */
#include <stdint.h>

static inline int count_runs(seq_t x)
{
    /* Count runs: number of changes between adjacent bits plus one. */
    if (x == 0) return 1;          /* not used for positive integers */
    int cnt = 1;
    seq_t prev = x & 1;
    x >>= 1;
    while (x) {
        seq_t cur = x & 1;
        if (cur != prev) ++cnt;
        prev = cur;
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t found = 0;
    while (1) {
        if (count_runs(candidate) % 5 == 3) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;               /* next integer */
    }
}
