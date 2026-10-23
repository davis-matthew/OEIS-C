/*
 * A100051  A Chebyshev transform of 1,1,1,...
 * https://oeis.org/A100051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(0)=1; a(1)=1; a(2)=-1; a(n)=a(n-1)-a(n-2) for n>2. This
 *            sequence is periodic after a(0) with period 6: [1, -1, -2, -1,
 *            1, 2] repeated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t A100051_pat[6] = {
    (seq_t)1,  (seq_t)-1,  (seq_t)-2,
    (seq_t)-1, (seq_t)1,   (seq_t)2
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;              /* a(0) */
    return A100051_pat[(n - 1) % 6];          /* pattern for n>=1 */
}
