/*
 * A212566  Number of (w,x,y,z) with all terms in {1,...,n} and w+x=3y+3z.
 * https://oeis.org/A212566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) equals the number of quadruples (w, x, y, z) with each in
 *            {1,…,n} that satisfy w + x = 3y + 3z. The count can be computed
 *            by summing over all possible values of S = y+z: a(n) =
 *            Σ_{S=2}^{⌊2n/3⌋} count_pairs(S, n) * count_pairs(3S, n) where
 *            count_pairs(s, n) is the number of pairs (u, v)∈{1,…,...
 * Range:     u8 n <= 13, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 3) return 0;

    seq_t result = 0;
    seq_t n_seq = (seq_t)n;
    idx_t maxS = (2*n) / 3;

    for (idx_t S = 2; S <= maxS; ++S) {
        seq_t count_yz;
        if (S <= n + 1)
            count_yz = (seq_t)(S - 1);
        else
            count_yz = (seq_t)(2 * n - S + 1);

        seq_t sumW = (seq_t)(3 * S);
        seq_t count_wx;
        if (sumW <= n_seq + 1)
            count_wx = sumW - 1;
        else
            count_wx = 2 * n_seq - sumW + 1;

        result += count_yz * count_wx;
    }

    return result;
}
