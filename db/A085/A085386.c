/*
 * A085386  E.g.f. cosh(x+x^2/2).
 * https://oeis.org/A085386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of involutions with an even number of cycles
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence can be computed by the following O(n) recurrences: */
/* Let E(n) = # involutions on n elements with an even number of cycles */
/* Let O(n) = # involutions on n elements with an odd number of cycles */
/* Then:  E(0)=1, O(0)=0 */
/*        E(n) = O(n-1) + (n-1)*O(n-2)  for n≥1 */
/*        O(n) = E(n-1) + (n-1)*E(n-2)  for n≥1 */
/* The desired term a(n) is E(n). */

seq_t a(idx_t n)
{
    /* Handle the trivial case n==0 immediately */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Initialize recurrence variables:
       E_prev2 = E(n-2), E_prev1 = E(n-1) */
    seq_t E_prev2 = (seq_t)0, E_prev1 = (seq_t)1;   /* E(-1)=0, E(0)=1 */

    /* O_prev2 = O(n-2), O_prev1 = O(n-1) */
    seq_t O_prev2 = (seq_t)0, O_prev1 = (seq_t)0;   /* O(-1)=0, O(0)=0 */

    /* Current values */
    seq_t E_cur = 0, O_cur = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t k = (seq_t)(i - 1);       /* k = n-1 as seq_t */

        /* Compute E(i) and O(i) using the recurrences */
        E_cur = O_prev1 + k * O_prev2;
        O_cur = E_prev1 + k * E_prev2;

        /* Shift the variables for next iteration */
        E_prev2 = E_prev1;
        E_prev1 = E_cur;
        O_prev2 = O_prev1;
        O_prev1 = O_cur;
    }

    /* At the end of the loop, E_cur = E(n) is the desired value */
    return E_cur;
}
