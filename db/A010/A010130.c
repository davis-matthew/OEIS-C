/*
 * A010130  Continued fraction for sqrt(32).
 * https://oeis.org/A010130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 5; for n > 0, a(n) = 10 if n % 4 == 0, otherwise a(n) = 1
 * Range:     u8 n <= 9832, u16 n <= 9832, u32 n <= 9832, u64 n <= 9832,
 *            u128 n <= 9832, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows the recurrence a(n) = a(n-4) for n > 4 with initial terms
   a(0)=5, a(1)=1, a(2)=1, a(3)=1, a(4)=10. */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)5;
    }
    /* n > 0 */
    if ((n % (idx_t)4) == 0) {
        return (seq_t)10;
    } else {
        return (seq_t)1;
    }
}
