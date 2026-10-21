/*
 * A088172  First differences of A019300.
 * https://oeis.org/A088172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A088172: first differences of A019300 (offset 1).
   The sequence satisfies the recurrence
        a(1) = 1
        for n ≥ 1
        a(n+1) = 2·a(n) + d(n)
   where the small increment d(n) is defined by the binary properties
   of n:
     • if n is even:        d(n) = (−1)^{popcount(n)}
     • if n ≡ 1 (mod 4):   d(n) = 0
     • if n ≡ 3 (mod 4):   let k = (n – 3) / 4
          – if k ≡ 1 or 5 (mod 6) then d(n) = 0
          – otherwise           d(n) = (−1)^{popcount(n)}
   Here popcount(n) is the number of 1‑bits in the binary expansion of n.
   This definition reproduces the first differences of A019300
   and generates the terms listed in the OEIS entry.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t val = (seq_t)1;

    for (idx_t k = 1; k < n; ++k) {
        int8_t d = 0;

        /* even k */
        if ((k & (unsigned long long)1ULL) == 0) {
            unsigned int pc = __builtin_popcountll(k);
            d = (pc & 1) ? -1 : 1;    /* (−1)^{popcount(k)} */
        }
        /* odd k with k ≡ 1 (mod 4) */
        else if ((k & (unsigned long long)3ULL) == 1) {
            d = 0;
        }
        /* odd k with k ≡ 3 (mod 4) */
        else {
            uint64_t kk = (k - 3) >> 2;      /* (k-3)/4 */
            uint64_t r = kk % 6ULL;
            if (r == 1 || r == 5) {
                d = 0;
            } else {
                unsigned int pc = __builtin_popcountll(k);
                d = (pc & 1) ? -1 : 1;
            }
        }

        val = (val << 1) + (seq_t)d;  /* val = 2*val + d */
    }

    return val;
}
