/*
 * A133672  a(n) = a(n-1) - 81*a(n-2), a(0)=1, a(1)=9.
 * https://oeis.org/A133672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=9, a(n)=a(n-1)-81*a(n-2) for n≥2
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 10, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)9;   /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 - ((seq_t)81 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
