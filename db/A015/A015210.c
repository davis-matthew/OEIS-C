/*
 * A015210  Sum of Gaussian binomial coefficients for q=19.
 * https://oeis.org/A015210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
  Formula: a(0) = 1
           a(1) = 2
           a(n) = 2*a(n-1) + a(n-2)*((19^(n-1)) - 1)  for n ≥ 2
*********************************************************************/

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* prev   = a(n-2)
       curr   = a(n-1)
       pow19  = 19^(n-1), starting with 19^1 for i = 2
    */
    seq_t prev = (seq_t)1;      /* a(0) */
    seq_t curr = (seq_t)2;      /* a(1) */
    seq_t pow19 = (seq_t)19;    /* 19^(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term = pow19 - (seq_t)1;          /* (19^(i-1) - 1) */
        seq_t new_val = (seq_t)2 * curr + prev * term;

        /* Prepare for next iteration */
        prev = curr;
        curr = new_val;

        /* Advance power: pow19 = 19^i for next i+1 */
        pow19 *= (seq_t)19;
    }

    return curr;
}
