/*
 * A236438  a(n) = n*a(n-1) + (-1)^n for n>0, a(0)=2.
 * https://oeis.org/A236438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * a(n-1) + (-1)^n with a(0) = 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)2;
    }
    seq_t result = 2;              /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        result = (seq_t)i * result;  /* i * a(i-1) */
        if (i & 1) {                 /* odd i -> subtract 1 */
            result = result - 1;
        } else {                     /* even i -> add 1 */
            result = result + 1;
        }
    }
    return result;
}
