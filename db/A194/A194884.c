/*
 * A194884  Write n = C(i,4)+C(j,3)+C(k,2)+C(l,1) with i>j>k>l>=0; sequence
 *          gives k values.
 * https://oeis.org/A194884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) returns the k for the unique combinatorial representation n
 *            = C(i,4) + C(j,3) + C(k,2) + C(l,1) with i > j > k > l >= 0
 * Range:     u8 n <= 118, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom_small(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;        /* use symmetry */
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res /= i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n) {
    seq_t rest = (seq_t)n;
    /* find i : largest such that C(i,4) <= rest */
    idx_t i = 0;
    while (1) {
        seq_t c4 = binom_small(i, 4);
        if (c4 > rest) break;
        i++;
    }
    i--;                                    /* go back to last acceptable i */
    rest -= binom_small(i, 4);

    /* find j : largest such that C(j,3) <= rest */
    idx_t j = 0;
    while (1) {
        seq_t c3 = binom_small(j, 3);
        if (c3 > rest) break;
        j++;
    }
    j--;
    rest -= binom_small(j, 3);

    /* find k : largest such that C(k,2) <= rest */
    idx_t k = 0;
    while (1) {
        seq_t c2 = binom_small(k, 2);
        if (c2 > rest) break;
        k++;
    }
    k--;
    /* The remainder rest is l (which is not returned). */
    return (seq_t)k;
}
