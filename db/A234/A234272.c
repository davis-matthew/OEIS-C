/*
 * A234272  G.f.: (1+4*x+x^2)/(1-4*x+x^2).
 * https://oeis.org/A234272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)-a(n-2) for n > 2 Initial values: a(0)=1, a(1)=8,
 *            a(2)=32
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)32;

    /* Use two running variables to apply the recurrence. */
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)8;   /* a(1) */
    seq_t a_prev   = (seq_t)32; /* a(2) */
    seq_t a_curr   = a_prev;    /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)4 * a_prev - a_prev1;
        a_prev1 = a_prev;
        a_prev  = a_curr;
    }

    return a_curr;
}
