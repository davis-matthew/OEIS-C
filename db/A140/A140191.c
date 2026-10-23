/*
 * A140191  Fix e = 3; a(n) gives number of multiples ke (0 <= k <= n/e) which
 *          are children of n.
 * https://oeis.org/A140191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of multiples k*3 (0 <= k <= n/3) that are bitwise
 *            submasks of n. Implementation uses a DP over the set bits of n.
 *            The DP counts submasks of n whose value modulo 3 equals 0. This
 *            yields the required sequence for any n
 * Range:     u8 n <= 1022, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* dp[r] = number of submasks processed so far whose value ≡ r (mod 3) */
    seq_t dp[3] = {1, 0, 0};          /* empty submask counts as residue 0 */
    seq_t ndp[3];
    idx_t pos = 0;
    idx_t bits = n;                   /* iterate over set bits of n */

    while (bits != 0) {
        if (bits & 1) {               /* if bit 'pos' of n is 1, it can be taken or not */
            int val = (pos & 1) ? 2 : 1;   /* 2^pos mod 3: 1 for even pos, 2 for odd pos */
            ndp[0] = ndp[1] = ndp[2] = 0;

            for (int r = 0; r < 3; ++r) {
                seq_t ways = dp[r];
                if (!ways) continue;

                /* leave bit unset */
                ndp[r] += ways;

                /* set bit, update residue */
                int nr = r + val;
                if (nr >= 3) nr -= 3;
                ndp[nr] += ways;
            }
            dp[0] = ndp[0];
            dp[1] = ndp[1];
            dp[2] = ndp[2];
        }
        bits >>= 1;
        ++pos;
    }
    return dp[0];  /* submasks divisible by 3, including the empty one (0) */
}
