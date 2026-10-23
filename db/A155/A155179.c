/*
 * A155179  a(n) = 4*a(n-1)+a(n-2), n>2; a(0)=1, a(1)=3, a(2)=12.
 * https://oeis.org/A155179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1)+a(n-2), a(0)=1, a(1)=3, a(2)=12 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)12;

    /* Iteratively compute for n ≥ 3 */
    seq_t a_prev2 = (seq_t)3;   /* a(1) */
    seq_t a_prev1 = (seq_t)12;  /* a(2) */
    seq_t a_curr;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = ((seq_t)4) * a_prev1 + a_prev2; /* a(i) = 4*a(i-1)+a(i-2) */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
