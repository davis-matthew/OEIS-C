/*
 * A020962  a(n) = Sum_{k >= 1} floor((1+sqrt(2))^(n-k)).
 * https://oeis.org/A020962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n-1} floor((1+sqrt(2))^i)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the recurrence for a_i = r^i + s^i where r=1+sqrt2, s=1-sqrt2,
   namely a_i = 2*a_{i-1} + a_{i-2} with a_0 = a_1 = 2.
   Then floor(r^i) = a_i - (i%2==0 ? 1 : 0). */
/* The function computes a(n) iteratively for any n >= 1. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, not used */

    /* a_0 = 2, a_1 = 2 */
    seq_t a_prev2 = (seq_t)2;              /* a_{i-2} */
    seq_t a_prev1 = (seq_t)2;              /* a_{i-1} */
    seq_t acc = (seq_t)1;                  /* floor(r^0) */

    /* Loop over i = 1 .. n-1 */
    for (idx_t i = 1; i < n; ++i) {
        seq_t a_curr;
        if (i == 1) {
            a_curr = a_prev1;              /* a_1 */
        } else {
            a_curr = a_prev1 * (seq_t)2 + a_prev2; /* a_i = 2*a_{i-1} + a_{i-2} */
        }

        /* floor(r^i) */
        seq_t floor_i = a_curr - ((i % 2 == 0) ? (seq_t)1 : (seq_t)0);

        acc += floor_i;

        /* shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return acc;
}
