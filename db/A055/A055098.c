/*
 * A055098  Number of distinct anagrams of digits of n without leading zeros.
 * https://oeis.org/A055098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   a(n) = number of distinct permutations of the decimal digits of
 *            n that do not start with a zero
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No additional typedefs or includes are necessary; seq_t and idx_t are
   defined by the build. */
seq_t a(idx_t n)
{
    /* Count decimal digits of n. */
    uint8_t cnt[10] = {0};
    idx_t tmp = n;
    /* Handle the special case n == 0, which never occurs for the
       sequence's offset but is harmless. */
    if (tmp == 0)
        cnt[0] = 1;
    else {
        while (tmp > 0) {
            cnt[tmp % 10]++;          /* tmp % 10 is idx_t, fits in uint8_t */
            tmp /= 10;
        }
    }

    /* Total number of digits. */
    int L = 0;
    for (int d = 0; d < 10; ++d)
        L += cnt[d];

    /* Compute factorial(L). */
    seq_t total = 1;
    for (int i = 2; i <= L; ++i)
        total *= (seq_t)i;

    /* Divide by factorials of each digit count. */
    for (int d = 0; d < 10; ++d) {
        int c = cnt[d];
        if (c > 1) {
            seq_t denom = 1;
            for (int j = 2; j <= c; ++j)
                denom *= (seq_t)j;
            total /= denom;
        }
    }

    /* Count permutations starting with zero (if any zero digit exists). */
    seq_t leading = 0;
    if (cnt[0] > 0) {
        int L0 = L - 1;                 /* one zero fixed at front */
        seq_t tmp_fact = 1;
        for (int i = 2; i <= L0; ++i)
            tmp_fact *= (seq_t)i;

        /* Adjust counts: zero count reduced by one, others unchanged. */
        for (int d = 0; d < 10; ++d) {
            int c = (d == 0) ? cnt[0] - 1 : cnt[d];
            if (c > 1) {
                seq_t denom = 1;
                for (int j = 2; j <= c; ++j)
                    denom *= (seq_t)j;
                tmp_fact /= denom;
            }
        }
        leading = tmp_fact;
    }

    return total - leading;
}
