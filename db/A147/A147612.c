/*
 * A147612  If n is a Jacobsthal number then 1 else 0.
 * https://oeis.org/A147612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a Jacobsthal number (defined by J_0 = 0, J_1 =
 *            1, J_k = J_{k-1} + 2*J_{k-2}) else 0. a(0)=1, a(1)=1, a(2)=0,
 *            a(3)=1,
 * Range:     u8 n <= 9743, u16 n <= 9743, u32 n <= 9743, u64 n <= 9743,
 *            u128 n <= 9743, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two smallest Jacobsthal numbers explicitly. */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Generate subsequent Jacobsthal numbers until exceeding n. */
    idx_t prev2 = 0;           /* J_{k-2}  */
    idx_t prev1 = 1;           /* J_{k-1}  */

    for (;;)
    {
        /* If prev1 > n then we have already passed all Jacobsthal
           numbers <= n (this cannot happen after the first iteration
           because prev1 <= n by construction); the check is kept as a
           safety guard. */
        if (prev1 > n)
            break;

        /* Detect whether the next value would exceed n without
           overflowing.  2*prev2 > n - prev1  ⇔  prev2 > (n - prev1)/2. */
        if (prev2 > 0 && prev2 > (n - prev1) / 2)
            break;

        /* Compute the next Jacobsthal number. */
        idx_t next = prev1 + (idx_t)2 * prev2;

        if (next == n)
            return (seq_t)1;

        if (next > n)
            break;

        /* Shift the two most recent terms for the next iteration. */
        prev2 = prev1;
        prev1 = next;
    }

    return (seq_t)0;
}
