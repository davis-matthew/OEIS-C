/*
 * A152265  a(n) = ((8 + sqrt(7))^n + (8 - sqrt(7))^n)/2.
 * https://oeis.org/A152265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((8 + sqrt(7))^n + (8 - sqrt(7))^n)/2. Recurrence: a(n) =
 *            16*a(n-1) - 57*a(n-2), with a(0)=1, a(1)=8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)8;
    const seq_t C16 = (seq_t)16;
    const seq_t C57 = (seq_t)57;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t temp = C16 * a1 - C57 * a0;
        a0 = a1;
        a1 = temp;
    }
    return a1;
}
