/*
 * A082569  a(1)=2; a(n)=ceiling(n*(a(n-1)-1/a(n-1))).
 * https://oeis.org/A082569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(2) = 3; for n > 2, a(n) = 4 * n! / 3. Recurrence
 *            used: a(3) = 8; for n >= 4, a(n) = a(n-1) * n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first three terms separately */
    if (n == 1) {
        return (seq_t)2;
    }
    if (n == 2) {
        return (seq_t)3;
    }
    if (n == 3) {
        return (seq_t)8;
    }

    /* For n >= 4 compute a(n) = 8 * 4 * 5 * ... * n  */
    seq_t result = (seq_t)8;      /* a(3) */
    for (idx_t k = 4; k <= n; ++k) {
        result = result * (seq_t)k;
    }
    return result;
}
