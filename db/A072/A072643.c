/*
 * A072643  Half of the binary width of the terms of A014486, the number of
 *          digits in A063171(n)/2.
 * https://oeis.org/A072643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min k such that ∑_{i=0}^{k} Catalan(i) > n (Catalan
 *            numbers C_0=1, C_{m+1}=C_m·(4m+2)/(m+2))
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cum   = 0;     /* cumulative sum of Catalan numbers */
    seq_t C     = 1;     /* C_0 = 1 */
    idx_t m     = 0;     /* current index */
    seq_t sn    = n;     /* promote n for comparison */

    while (1) {
        cum += C;
        if (sn < cum)
            return (seq_t)m;

        /* compute next Catalan number */
        seq_t factor = ((seq_t)m) * 4 + 2;      /* 4*m + 2 */
        seq_t nextC  = C * factor;
        C = nextC / ((seq_t)(m + 2));          /* divide by m + 2 */
        m++;
    }
}
