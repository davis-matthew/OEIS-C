/*
 * A093467  a(1) = 1, a(2) = 2; for n >= 2, a(n+1) = a(n) + Sum_{i = 1..n}
 *          (a(i) - a(1)).
 * https://oeis.org/A093467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A093467: a(1)=1, a(2)=2, a(3)=3; a(n)=3*a(n-1)-a(n-2)-1 for n>3 */
seq_t a(idx_t n)
{
    /* Offset is 1; behaviour for n<1 is irrelevant. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    seq_t a_prev2 = (seq_t)2;   /* a(n-2) */
    seq_t a_prev1 = (seq_t)3;   /* a(n-1) */
    /* Compute a(n) for n ≥ 4 */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t a_curr = (seq_t)3 * a_prev1 - a_prev2 - (seq_t)1;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
