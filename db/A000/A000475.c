/*
 * A000475  Rencontres numbers: number of permutations of [n] with exactly 4
 *          fixed points.
 * https://oeis.org/A000475
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (-1)^n*binomial(n,4), a(4) = 1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 4) return 0;

    seq_t result = 1;
    /* binomial(i,k) for k = 4,3,2,1 at i = 4 */
    seq_t c4 = 1, c3 = 4, c2 = 6, c1 = 4;

    for (idx_t i = 5; i <= n; i++) {
        /* Pascal step: binomial(i,k) = binomial(i-1,k) + binomial(i-1,k-1) */
        c4 += c3;
        c3 += c2;
        c2 += c1;
        c1 += 1;

        result = (seq_t)i * result;
        if (i % 2 == 1)
            result -= c4;
        else
            result += c4;
    }

    return result;
}
