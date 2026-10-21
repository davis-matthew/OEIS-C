/*
 * A064874  Second of four sequences representing the lexicographical minimal
 *          decomposition of n in 4 squares: n = A064873(n)^2 + a(n)^2 +
 *          A064875(n)^2 + A064876(n)^2.
 * https://oeis.org/A064874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the second element of the lexicographically * minimal
 *            sorted quadruple (x,y,z,w) of non‑negative integers such that *
 *            x² + y² + z² + w² = n. The quadruple is considered sorted so * x
 *            ≤ y ≤ z ≤ w. This is the second component of the decomposition *
 *            used in A064873, A064874, A064875, A064876
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for seq_t values.  Returns floor(√x). */
static inline seq_t seq_isqrt(seq_t val) {
    seq_t low = 0, high = 1;
    /* grow high until high² > val */
    while ((unsigned __int128)high * high <= (unsigned __int128)val) {
        high <<= 1;
    }
    low = high >> 1;
    /* binary search for the floor square root */
    while (low < high) {
        seq_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * mid <= (unsigned __int128)val) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n) {
    /* n is an index (uint64_t).  Treat it as a seq_t for calculations. */
    seq_t sn = (seq_t)n;

    /* n = 0 has the trivial decomposition (0,0,0,0). */
    if (sn == 0) return 0;

    for (idx_t x = 0; (unsigned __int128)x * x <= (unsigned __int128)sn; ++x) {
        seq_t rem1 = sn - (seq_t)x * (seq_t)x;
        for (idx_t y = x; (unsigned __int128)y * y <= (unsigned __int128)rem1; ++y) {
            seq_t rem2 = rem1 - (seq_t)y * (seq_t)y;
            for (idx_t z = y; (unsigned __int128)z * z <= (unsigned __int128)rem2; ++z) {
                seq_t rem3 = rem2 - (seq_t)z * (seq_t)z;
                seq_t w = seq_isqrt(rem3);
                if ((unsigned __int128)w * w == (unsigned __int128)rem3 && w >= z) {
                    return (seq_t)y;   /* second component of the minimal tuple */
                }
            }
        }
    }
    /* By Legendre's three-square theorem every non‑negative integer
     * is representable as a sum of four squares, so this point is
     * unreachable for the domain of the sequence. */
    return 0;
}
