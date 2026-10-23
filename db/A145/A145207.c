/*
 * A145207  Numbers n such that there exists x in N with (x+67)^3-x^3=n^2.
 * https://oeis.org/A145207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 1030190*a(n+1) - a(n) Initial values: a(1) = 139159,
 *            a(2) = 143360071051 The function returns the n-th term of
 *            A145207 for n>=1
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        /* Offset is 1, behaviour for n<1 is undefined, return 0 */
        return (seq_t)0;
    }
    const seq_t FIRST  = 139159;
    const seq_t SECOND = 143360071051ULL;
    if (n == 1) return FIRST;
    if (n == 2) return SECOND;

    seq_t prev2 = FIRST;   /* a(n-2) */
    seq_t prev1 = SECOND;  /* a(n-1) */
    seq_t next;

    /* Compute up to a(n) iteratively */
    for (idx_t i = 3; i <= n; ++i) {
        next = (seq_t)1030190 * prev1 - prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
