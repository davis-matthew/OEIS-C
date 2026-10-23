/*
 * A041199  Denominators of continued fraction convergents to sqrt(110).
 * https://oeis.org/A041199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the continued fraction of sqrt(110):
   a(0) = 1, a(1) = 2.
   For n >= 2:
       if n is even  → a(n) = 20*a(n-1) + a(n-2)
       if n is odd   → a(n) =  2*a(n-1) + a(n-2)
   This follows from the periodic continued fraction [10; (2,20)].
*/
seq_t a(idx_t n)
{
    /* handle the two base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* q_{-1} = 1, q_0 = 1, q_1 = 2 for the first two convergents */
    seq_t q_prev2 = (seq_t)1;  /* a(0) */
    seq_t q_prev1 = (seq_t)2;  /* a(1) */
    seq_t q = q_prev1;        /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        /* coefficient alternates 20 (even) and 2 (odd) starting at i=2 */
        seq_t coef = (i & 1) ? (seq_t)2 : (seq_t)20;
        q = coef * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return q;
}
