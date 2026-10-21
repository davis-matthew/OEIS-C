/*
 * A063354  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          81 ).
 * https://oeis.org/A063354
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(2) = -1 a(3) = 164 a(4) = 356 For n > 4: a(n) = 2*a(n-1) -
 *            a(n-2) (This linear recurrence has closed‑form solution a(n) =
 *            192*n - 412 for n ≥ 4.) The function implements the closed form
 *            directly for speed
 * Range:     i8 n <= 2, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2)           return (seq_t)-1;
    if (n == 3)           return (seq_t)164;
    /* For n ≥ 4 use a(n) = 192*n - 412 */
    seq_t sn = (seq_t)n;              /* cast index to seq_t first */
    seq_t res = (seq_t)192 * sn - (seq_t)412;
    return res;
}
