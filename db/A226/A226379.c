/*
 * A226379  a(5n) = 2*n*(2*n+1), a(5n+1) = (2*n-3)*(2*n+5), a(5n+2) =
 *          (2*n-1)*(2*n+3), a(5n+3) = (2*n+2)*(2*n+1), a(5n+4) =
 *          (2*n+1)*(2*n+3).
 * https://oeis.org/A226379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(5n) = 2*n*(2*n+1), a(5n+1) = (2*n-3)*(2*n+5), a(5n+2) =
 *            (2*n-1)*(2*n+3), a(5n+3) = (2*n+2)*(2*n+1), a(5n+4) =
 *            (2*n+1)*(2*n+3)
 * Range:     i8 n <= 27, i16 n <= 452, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the block index k = floor(n/5) and the remainder r = n % 5. */
    idx_t k = n / 5;
    idx_t r = n % 5;
    /* Convert to seq_t for arithmetic. */
    seq_t nk = (seq_t)k;

    switch (r) {
        case 0:  /* n = 5k */
            return 2 * nk * (2 * nk + 1);
        case 1:  /* n = 5k+1 */
            return (2 * nk - 3) * (2 * nk + 5);
        case 2:  /* n = 5k+2 */
            return (2 * nk - 1) * (2 * nk + 3);
        case 3:  /* n = 5k+3 */
            return (2 * nk + 2) * (2 * nk + 1);
        case 4:  /* n = 5k+4 */
            return (2 * nk + 1) * (2 * nk + 3);
        default: /* unreachable: r is 0..4 */
            return 0;
    }
}
