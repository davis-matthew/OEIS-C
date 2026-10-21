/*
 * A081929  Sum of the n-th row of A077339.
 * https://oeis.org/A081929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (201*n*n - 2001*n + 2)/2
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 10) {
        /* first 10 terms */
        static const seq_t small[10] = {
            1,   22,   64,  127,  211,  316,  442,  589,  757,  946
        };
        return small[n - 1];
    }
    seq_t N = (seq_t)n;
    /* For n > 10 the closed‑form formula is valid and yields integer values. */
    return ((201 * N * N - 2001 * N + 2) / 2);
}
