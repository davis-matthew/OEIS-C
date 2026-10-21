/*
 * A203009  (n-1)-st elementary symmetric function of first n Lucas numbers,
 *          starting with L(0)=2.
 * https://oeis.org/A203009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{i=0}^{n-2} L_i + L_{n-1} * a(n-1) where L_0 = 2,
 *            L_1 = 1, L_{k} = L_{k-1} + L_{k-2} (Lucas numbers). This follows
 *            from the recurrence for elementary symmetric functions:
 *            e_{k}(S∪{x}) = e_{k}(S) + x * e_{k-1}(S). Here S contains the
 *            first n‑1 Lucas numbers and k = n‑1. For n ≥ 2, e...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* offset starts at 1, undefined for 0 */
    if (n == 1) return 1;       /* a(1) = 1 */

    /* Current Lucas numbers: L_prev = L_{k-2}, L_curr = L_{k-1}  */
    seq_t L_prev = 2;           /* L_0          */
    seq_t L_curr = 1;           /* L_1          */

    /* prod_prev = product of L_0 … L_{k-2} (empty product for k = 2) */
    seq_t prod_prev = L_prev;   /* product L_0      */
    seq_t a_prev  = 1;          /* a(1)            */

    /* Compute a(k) for k = 2 … n */
    idx_t k;
    for (k = 2; k <= n; ++k) {
        /* a(k) = prod_prev + L_curr * a_prev       */
        seq_t a_curr = prod_prev + (seq_t)L_curr * a_prev;

        /* Update product for the next step: now product of first k-1 Lucas numbers */
        prod_prev = prod_prev * (seq_t)L_curr;

        /* Advance Lucas numbers to next pair */
        seq_t L_next = L_prev + L_curr;
        L_prev = L_curr;
        L_curr = L_next;

        /* Prepare for next iteration */
        a_prev = a_curr;
    }

    return a_prev;   /* a(n) */
}
