/*
 * A078697  Duplicate of A066629.
 * https://oeis.org/A078697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = a(n-1) + a(n-2) + (n%2==0 ? 2 : 1), with a(0)=1, a(1)=2
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 18, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)2;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev2 + prev1;
        curr += (i & 1 ? (seq_t)1 : (seq_t)2);   /* add 1 if i is odd, 2 if even */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
