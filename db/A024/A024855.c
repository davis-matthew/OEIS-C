/*
 * A024855  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (natural numbers), t = A023531.
 * https://oeis.org/A024855
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{⌊n/2⌋} i · t(n‑i+1), where t(j) = 1 iff j+1 is
 *            a triangular number, otherwise t(j) = 0. Thus a(n) counts the
 *            weighted occurrences of (triangular‑1) within the window
 *            [n‑⌊n/2⌋+1, n]
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)  /* offset is 2; values below are undefined, return 0 */
        return 0;

    /* window limits for j = n‑i+1 */
    idx_t k   = n / 2;            /* floor(n/2) */
    idx_t low = n - k + 2;        /* minimal triangular number T in the window */
    idx_t high = n + 1;           /* maximal T in the window */

    seq_t sum = 0;
    for (idx_t m = 1; ; ++m) {
        idx_t T = m * (m + 1) / 2;   /* triangular number m(m+1)/2 */
        if (T > high)
            break;
        if (T >= low)
            sum += (seq_t)(n - T + 2);
    }
    return sum;
}
