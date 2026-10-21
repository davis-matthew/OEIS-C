/*
 * A201049  Related to ranking of teams in the canonical symmetric knockout
 *          tournament of order n.
 * https://oeis.org/A201049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 + 2^((n-1)/2) for odd n * a(2) = 4 * a(4) = 3 * a(n) =
 *            5 * 2^(n/2-3) for even n ≥ 6 * Uses a small pre‑computed powers
 *            of two array to keep the * computation fast and independent of
 *            idx_t size
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* pre‑compute 2^k for k = 0 … 5000 once */
    static seq_t pow2[5001];
    static unsigned init = 0;
    if (!init) {
        pow2[0] = 1;
        for (idx_t i = 1; i <= 5000; ++i)
            pow2[i] = pow2[i - 1] * 2;
        init = 1;
    }

    if (n == 0)   /* not used, but safe */
        return 0;

    if ((n & 1) == 0) {                 /* even n */
        if (n == 2)
            return 4;
        if (n == 4)
            return 3;
        /* for n ≥ 6 : a(n) = 5 * 2^(n/2-3) */
        idx_t exp = n / 2 - 3;
        return pow2[exp] * 5;
    } else {                            /* odd n */
        idx_t exp = (n - 1) / 2;
        return pow2[exp] + 1;
    }
}
