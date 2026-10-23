/*
 * A008489  Expansion of (1-x^7)/(1-x)^7.
 * https://oeis.org/A008489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * n * (84 + 35 * n^2 + n^4) / 120 (n > 0) a(0) = 1
 *            Generates the expansion of (1 - x^7) / (1 - x)^7 (OEIS A008489)
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 117, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    seq_t tn = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = tn * tn;           /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    seq_t inner = (seq_t)84
                + (seq_t)35 * n2
                + n4;           /* 84 + 35*n^2 + n^4 */
    seq_t num = (seq_t)7 * tn * inner; /* 7*n*(...) */
    return num / (seq_t)120;     /* division is exact */
}
