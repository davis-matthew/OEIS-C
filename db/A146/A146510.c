/*
 * A146510  Numbers congruent to {1, 4} mod 15.
 * https://oeis.org/A146510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k-1) = 15*(k-1)+1, a(2k) = 15*(k-1)+4
 * Range:     u8 n <= 34, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n >= 1; behavior for n == 0 is unspecified. */
    if (n == 0) return 0;

    /* Use unsigned arithmetic for safe underflow/overflow handling. */
    if ((n & 1ULL) == 1) {          /* n is odd: a(2k-1) */
        idx_t k = (n - 1) / 2;      /* k = (n-1)/2 = (2k-2)/2 = k-1 */
        return (seq_t)k * (seq_t)15 + (seq_t)1;
    } else {                        /* n is even: a(2k) */
        idx_t k = (n - 2) / 2;      /* k = (n-2)/2 = (2k-2)/2 = k-1 */
        return (seq_t)k * (seq_t)15 + (seq_t)4;
    }
}
