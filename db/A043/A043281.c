/*
 * A043281  Maximal run length in base-7 representation of n.
 * https://oeis.org/A043281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximal run length of equal digits in the base-7
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t prev = 8;            /* sentinel value outside digit range 0-6 */
    idx_t curr = 0;            /* length of current run */
    idx_t maxr = 0;            /* maximum run length found */

    while (n > 0) {
        idx_t digit = n % 7;
        n /= 7;

        if (digit == prev) {
            curr++;                     /* extend current run */
        } else {
            if (curr > maxr)            /* finish previous run */
                maxr = curr;
            prev = digit;               /* start new run */
            curr = 1;
        }
    }
    if (curr > maxr)                    /* last run */
        maxr = curr;

    return (seq_t)maxr;
}
