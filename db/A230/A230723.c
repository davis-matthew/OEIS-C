/*
 * A230723  Number of non-equivalent ways to choose three points in an
 *          equilateral triangle grid of side n.
 * https://oeis.org/A230723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 3*n^5 - 3*n^4 + 10*n^3 + B + C)/288 where B =
 *            27*n^2 + 3*n - 9 if n is odd, B = 48*n otherwise and C = -32 if
 *            n == 1 (mod 3), C = 0 otherwise
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 71, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = n;        /* cast to seq_t for arithmetic */

    /* Compute powers of n */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;

    /* Positive parts of the numerator */
    seq_t term1 = n6;               /* n^6 */
    seq_t term2 = (seq_t)3 * n5;    /* 3*n^5 */
    seq_t term3 = (seq_t)10 * n3;   /* 10*n^3 */

    /* Compute B depending on parity of n */
    seq_t B;
    if (nn & 1) {                   /* n odd */
        B = (seq_t)27 * n2 + (seq_t)3 * nn - (seq_t)9;
    } else {                        /* n even */
        B = (seq_t)48 * nn;
    }

    /* Sum all positive contributions */
    seq_t sum = term1 + term2 + term3 + B;

    /* Subtract the negative term -3*n^4 */
    sum -= (seq_t)3 * n4;

    /* Apply C when n == 1 (mod 3) */
    if ((nn % 3) == 1) {
        sum -= (seq_t)32;          /* C = -32 */
    }

    /* Final division by 288 */
    return sum / (seq_t)288;
}
