/*
 * A166539  a(n) = (10*n + 7*(-1)^n + 5)/4.
 * https://oeis.org/A166539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7*(-1)^n + 5)/4
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence is OEIS A166539. */
seq_t a(idx_t n) {
    seq_t num = (seq_t)n * 10;       /* 10*n */
    num += 5;                        /* 10*n + 5 */
    if ((n & 1) == 0) {              /* n even: add 7 */
        num += 7;
    } else {                         /* n odd: subtract 7 */
        num -= 7;
    }
    return num / 4;                   /* division is exact */
}
