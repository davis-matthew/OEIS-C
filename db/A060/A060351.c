/*
 * A060351  If the binary expansion of n has k bits, let S be the subset of
 *          [k-1] such that i is in S if the i-th bit of n is a 1 (with the
 *          first bit being the least significant bit); a(n) is the number of
 *          permutations of [k] with descent set S.
 * https://oeis.org/A060351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn, look, tabf
 * Formula:   a(n) = number of permutations of [k] whose descent set
 *            corresponds to the lower (k‑1) bits of n, where k = ⌊log₂(n)⌋ +
 *            1
 * Range:     u8 n <= 84, u16 n <= 1108, u32 n <= 9663, u64 n <= 9663,
 *            u128 n <= 9663, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* compute number of bits k of n */
    unsigned int k = 0;
    idx_t t = n;
    do {
        ++k;
        t >>= 1;
    } while (t > 0);

    if (k == 1) return (seq_t)1;          /* only the permutation 1 */

    /* dp and newdp store counts for each possible rank of the
       last element in the current partial permutation. */
    seq_t dp[65] = {0};
    seq_t newdp[65] = {0};
    dp[0] = (seq_t)1;
    unsigned int len = 1;                 /* current length of permutations */

    for (unsigned int pos = 0; pos < k - 1; ++pos) {
        unsigned int cmp = (n >> pos) & 1U;   /* 0 = ascent, 1 = descent */
        unsigned int len_next = len + 1;

        if (cmp == 0) {                     /* ascent: new rank > previous rank */
            seq_t prefix = 0;
            for (unsigned int r = 0; r < len_next; ++r) {
                newdp[r] = prefix;
                if (r < len) prefix += dp[r];
            }
        } else {                            /* descent: new rank <= previous rank */
            seq_t suffix = 0;
            for (int r = (int)len_next - 1; r >= 0; --r) {
                if (r < (int)len) suffix += dp[r];
                newdp[r] = suffix;
            }
        }

        for (unsigned int i = 0; i < len_next; ++i) dp[i] = newdp[i];
        len = len_next;
    }

    /* sum all counts to obtain the total number of permutations */
    seq_t result = 0;
    for (unsigned int i = 0; i < len; ++i) result += dp[i];
    return result;
}
