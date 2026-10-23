/*
 * A047234  Numbers that are congruent to {0, 1, 4} mod 6.
 * https://oeis.org/A047234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { 6*k - 6 if n = 3*k - 2 (n % 3 == 1) , 6*k - 5 if n =
 *            3*k - 1 (n % 3 == 2) , 6*k - 2 if n = 3*k (n % 3 == 0) } where k
 *            = n / 3 or appropriate adjustment
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t mod = n % (idx_t)3;
    idx_t k;
    switch (mod) {
    case (idx_t)1:      /* n = 3*k - 2  → k = (n + 2) / 3 */
        k = (n + (idx_t)2) / (idx_t)3;
        return (seq_t)6 * k - (seq_t)6;
    case (idx_t)2:      /* n = 3*k - 1  → k = (n + 1) / 3 */
        k = (n + (idx_t)1) / (idx_t)3;
        return (seq_t)6 * k - (seq_t)5;
    default:            /* n = 3*k      → k = n / 3 */
        k = n / (idx_t)3;
        return (seq_t)6 * k - (seq_t)2;
    }
}
