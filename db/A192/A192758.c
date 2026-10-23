/*
 * A192758  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x in the reduction of p(n-1) by
 *            x^2->x+1. Recurrence: a(0)=0, b(0)=1 (reduced form of p(0)=1 is
 *            1 + 0*x) for i=1..n-1: a' = a + b b' = a + ⌊(i+4)/4⌋ Return a
 *            after the loop (this equals a(n) in the OEIS listing)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i;
    seq_t a_val = 0;
    seq_t b_val = 1;      /* constant term of reduced p(0) */

    for (i = 1; i < n; ++i) {
        seq_t c = (seq_t)((i + 4) / 4);   /* ⌊(i+4)/4⌋ */
        seq_t a_next = a_val + b_val;     /* a(n) = a(n-1) + b(n-1) */
        seq_t b_next = a_val + c;         /* b(n) = a(n-1) + ⌊(i+4)/4⌋ */
        a_val = a_next;
        b_val = b_next;
    }

    return a_val;
}
