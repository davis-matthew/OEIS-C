/*
 * A164588  a(n) = ((3 + sqrt(18))*(5 + sqrt(8))^n + (3 - sqrt(18))*(5 -
 *          sqrt(8))^n)/6.
 * https://oeis.org/A164588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((3 + sqrt(18))*(5 + sqrt(8))^n + (3 - sqrt(18))*(5 -
 *            sqrt(8))^n)/6 Recurrence: a(0) = 1, a(1) = 9 a(n) = 10*a(n-1) -
 *            17*a(n-2) for n > 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* Compute iteratively using the linear recurrence */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)9;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)10 - prev2 * (seq_t)17;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
