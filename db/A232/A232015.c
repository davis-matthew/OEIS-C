/*
 * A232015  Expansion of (1-2*x)/((1+2*x)*(1-3*x)).
 * https://oeis.org/A232015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 21, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 6*a(n-2) for n>1, a(0)=1, a(1)=-1
 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;

    seq_t prev0 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)-1;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t term = prev1 + (seq_t)((seq_t)prev0 * 6);
        prev0 = prev1;
        prev1 = term;
    }
    return prev1;
}
