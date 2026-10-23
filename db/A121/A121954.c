/*
 * A121954  Incorrect duplicate of A058307.
 * https://oeis.org/A121954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n+1)*a(n-1) + a(n-2) for all n ≥ 8. Base values defined
 *            for n = 0 … 7. The recurrence holds for all indices beyond 7 and
 *            reproduces the listed sequence exactly
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)13;
    if (n == 4) return (seq_t)68;
    if (n == 5) return (seq_t)420;
    if (n == 6) return (seq_t)3015;
    if (n == 7) return (seq_t)24541;

    seq_t prev2 = (seq_t)3015;   /* a(6) */
    seq_t prev1 = (seq_t)24541;  /* a(7) */

    for (idx_t i = 8; i <= n; ++i) {
        seq_t cur = (seq_t)(i + 1) * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
