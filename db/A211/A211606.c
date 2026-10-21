/*
 * A211606  Total number of inversions over all involutions of length n.
 * https://oeis.org/A211606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (n-1)*a(n-2) + A000085(n-2)*(n-1)^2 with initial
 *            values a(0)=a(1)=0. A000085(k) counts involutions:
 *            invol[0]=invol[1]=1, invol[k] = invol[k-1] + (k-1)*invol[k-2]
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 28, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Variables for involutions:
       inv_prevprev = invol[k-2]
       inv_prev     = invol[k-1]   */
    seq_t inv_prevprev = 1;   /* invol[0] */
    seq_t inv_prev     = 1;   /* invol[1] */

    /* Variables for a(n):
       a_prevprev = a[k-2]
       a_prev     = a[k-1] */
    seq_t a_prevprev = 0;   /* a[0] */
    seq_t a_prev     = 0;   /* a[1] */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t mult = (seq_t)(k - 1);

        /* Compute invol[k]  */
        seq_t new_invol = inv_prev + mult * inv_prevprev;

        /* Compute a[k]     */
        seq_t term3 = inv_prevprev * (mult * mult);
        seq_t new_a = a_prev + mult * a_prevprev + term3;

        /* Shift for next iteration */
        inv_prevprev = inv_prev;
        inv_prev     = new_invol;
        a_prevprev   = a_prev;
        a_prev       = new_a;
    }

    return a_prev;   /* a[n] after loop */
}
