/*
 * A054319  Duplicate of A006061.
 * https://oeis.org/A054319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = square of S(n) where S(1)=1, S(2)=11, and S(n)=10*S(n‑1)-S(n‑2) */
/* this is OEIS A006061 */

/* seq_t and idx_t are defined by the build system. */

seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)121;        /* 11^2 */
    }

    /* compute S(n) by recurrence */
    seq_t s_prev2 = (seq_t)1;    /* S(1) */
    seq_t s_prev1 = (seq_t)11;   /* S(2) */

    /* iterate from i=3 up to n */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t s = (seq_t)10 * s_prev1 - s_prev2;  /* S(i) = 10*S(i-1) - S(i-2) */
        s_prev2 = s_prev1;
        s_prev1 = s;
    }

    /* return square of S(n) */
    return s_prev1 * s_prev1;
}
