/*
 * A239573  Number of non-equivalent (mod D_3) ways to place 3
 *          indistinguishable points on a triangular grid of side n so that no
 *          two of them are adjacent.
 * https://oeis.org/A239573
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 3*n^5 - 39*n^4 + 10*n^3 + 456*n^2 - 1008*n +
 *            576)/288 + IF(n mod 2 = 1)*(3*n^2 - 5*n - 5)/32 + IF(n mod 3 =
 *            1)*2/9 (with offset 2, i.e. a(2) is the first value)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 102, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;         /* below the offset, return 0 */
    }

    /* Promote idx_t to seq_t for arithmetic. */
    seq_t N = (seq_t)n;

    /* Powers of N (using seq_t multiplication). */
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n3 * N;         /* n^4 */
    seq_t n5 = n4 * N;         /* n^5 */
    seq_t n6 = n5 * N;         /* n^6 */

    /* Numerator of the main fraction with denominator 288. */
    seq_t term1 = n6
                + (seq_t)3 * n5
                - (seq_t)39 * n4
                + (seq_t)10 * n3
                + (seq_t)456 * n2
                - (seq_t)1008 * N
                + (seq_t)576;

    /* Extra terms, also brought to denominator 288. */
    seq_t extra = (seq_t)0;

    if ((n & 1U) == 1U) {          /* n is odd */
        seq_t val = (seq_t)3 * n2
                  - (seq_t)5 * N
                  - (seq_t)5;
        /* (3*n^2 - 5*n - 5)/32  = 9*(3*n^2 - 5*n - 5)/288  */
        extra += (seq_t)9 * val;
    }

    if ((n % 3U) == 1U) {          /* n mod 3 == 1 */
        /* 2/9 = 64/288 */
        extra += (seq_t)64;
    }

    /* Final value: (term1 + extra) / 288 */
    seq_t numerator = term1 + extra;
    return numerator / (seq_t)288;
}
