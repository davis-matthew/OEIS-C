/*
 * A173193  a(n) = (2*10^n + 43)/9.
 * https://oeis.org/A173193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*10^n + 43)/9. Recurrence: a(1)=7, a(n)=10*a(n-1)-43
 *            for n>1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* The sequence has offset 1, so n must be >=1. Behaviour for n<1 is irrelevant. */
    if (n == 1) {
        return (seq_t)7;
    }
    seq_t val = 7;                 /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        val = val * 10 - 43;        /* a(i) = 10*a(i-1) - 43 */
    }
    return val;
}
