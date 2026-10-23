/*
 * A173134  a(n) = Sinh[(2n-1)ArcCosh[n]]^2.
 * https://oeis.org/A173134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sinh[(2n-1)arccosh(n)]^2, implemented via a(n) = (n^2-1)
 *            * U_{2n-2}(n)^2, where U_k(x) is the Chebyshev polynomial of the
 *            second kind defined by U_0(x)=1, U_1(x)=2x,
 *            U_{k+1}(x)=2x*U_k(x)-U_{k-1}(x)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 3, i64 n <= 5, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 directly. */
    if (n == (idx_t)0)
        return (seq_t)-1;

    /* For n = 1 the formula gives 0 directly. */
    if (n == (idx_t)1)
        return (seq_t)0;

    /* Convert index to seq_t once to avoid repeated casts. */
    seq_t n_seq = (seq_t)n;

    /* m = 2n-2 is the order of the Chebyshev U polynomial needed. */
    idx_t m = 2 * n - 2;           /* safe for n <= UINT64_MAX/2 */

    /* Compute U_m(n) iteratively. */
    seq_t U_prev0 = (seq_t)1;          /* U_0 */
    seq_t U_prev1 = 2 * n_seq;         /* U_1 */

    if (m == 0) {         /* In practice this happens only for n = 1,
                            which was already handled above. */
        /* No loop needed. */
    } else {
        for (idx_t i = 2; i <= m; ++i) {
            seq_t U = 2 * n_seq * U_prev1 - U_prev0;
            U_prev0 = U_prev1;
            U_prev1 = U;
        }
    }

    /* U_m is in U_prev1 after the loop (U_prev0 for the m==0 case). */
    seq_t U_m = (m == 0) ? U_prev0 : U_prev1;

    /* Final formula: a(n) = (n^2-1) * U_m^2. */
    seq_t result = (n_seq * n_seq - 1) * U_m * U_m;
    return result;
}
