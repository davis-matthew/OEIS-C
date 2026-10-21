/*
 * A216508  a(n) = 13*a(n-1) - 65*a(n-2) + 156*a(n-3) - 182*a(n-4) + 91*a(n-5)
 *          - 13*a(n-6), with initial terms 6, 13, 39, 130, 455, 1638.
 * https://oeis.org/A216508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 65*a(n-2) + 156*a(n-3) - 182*a(n-4) +
 *            91*a(n-5) - 13*a(n-6) Initial values: a(0)=6, a(1)=13, a(2)=39,
 *            a(3)=130, a(4)=455, a(5)=1638
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* static cache covering all n up to the expected limit (10 000) */
    static seq_t cache[10005];
    static idx_t cached = 0;                 /* number of values stored in cache */

    /* initialise the first six values if not already done */
    if (cached == 0) {
        cache[0] = 6;
        cache[1] = 13;
        cache[2] = 39;
        cache[3] = 130;
        cache[4] = 455;
        cache[5] = 1638;
        cached = 6;
    }

    /* If n is already cached, return it immediately */
    if (n < cached) {
        return cache[n];
    }

    /* Compute values up to n using the recurrence */
    for (idx_t i = cached; i <= n; ++i) {
        seq_t val = 13 * cache[i - 1]
                    - 65 * cache[i - 2]
                    + 156 * cache[i - 3]
                    - 182 * cache[i - 4]
                    + 91 * cache[i - 5]
                    - 13 * cache[i - 6];
        cache[i] = val;
    }
    cached = n + 1;

    return cache[n];
}
