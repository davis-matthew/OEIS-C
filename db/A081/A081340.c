/*
 * A081340  a(n) = (5^n+(-1)^n)/2.
 * https://oeis.org/A081340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (5^n + (-1)^n)/2
 * Recurrence: a(0)=1, a(1)=2, a(n) = 4*a(n-1) + 5*a(n-2)  (n>=2)
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t a0 = (seq_t)1;  // a(0)
    seq_t a1 = (seq_t)2;  // a(1)
    seq_t ai;
    for (idx_t i = 2; i <= n; ++i) {
        ai = (seq_t)4 * a1 + (seq_t)5 * a0;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
