/*
 * A043699  a(n)= A000129(n)*A000129(2*n) where A000129(n) are the Pell
 *          numbers.
 * https://oeis.org/A043699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Pell(n) * Pell(2*n) (Pell numbers with Pell(0)=0,
 *            Pell(1)=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A043699: a(n) = A000129(n) * A000129(2*n).  List starts at n=0. */

seq_t a(idx_t n)
{
    /* Two‑step variables for Pell numbers
     * p0 = Pell(i-2), p1 = Pell(i-1) */
    seq_t p0 = (seq_t)0;    /* Pell(0) */
    seq_t p1 = (seq_t)1;    /* Pell(1) */

    /* The index 2n we need Pell(2*n).  No overflow for realistic n. */
    idx_t two_n = n + n;

    /* Store Pell(n) and Pell(2n) when they are reached. */
    seq_t pn   = (seq_t)0;
    seq_t p2n  = (seq_t)0;

    /* Handle the trivial cases so the loop can stay simple. */
    if (n == (idx_t)0)     pn = (seq_t)0;
    else if (n == (idx_t)1)  pn = (seq_t)1;
    if (two_n == (idx_t)0)  p2n = (seq_t)0;
    else if (two_n == (idx_t)1) p2n = (seq_t)1;   /* never happens, but kept for completeness */

    /* Compute Pell numbers up to 2n.  The loop starts at i=2 because
     * Pell(0) and Pell(1) are already known.  The loop stops when i==2n. */
    for (idx_t i = (idx_t)2; i <= two_n; ++i) {
        seq_t p = (seq_t)2 * p1 + p0;  /* Pell(i) = 2*Pell(i-1)+Pell(i-2) */
        p0 = p1;
        p1 = p;
        if (i == n)   pn = p;
        if (i == two_n) p2n = p;
    }

    return pn * p2n;
}
