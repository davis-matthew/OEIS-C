/*
 * A089893  a(n) = (A001317(2n)-1)/4.
 * https://oeis.org/A089893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A001317(2n)-1)/4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the parity row of Pascal's triangle mod 2 for r = 2n
   as a binary number, then take (row-1)/4.  The parity condition
   binom(r,k) is odd iff every bit of k is ≤ the corresponding bit
   of r, i.e. k is a submask of r.  Enumerating all submasks
   (there are 2^popcount(r) of them) yields all terms with a '1'
   bit in the desired binary number. */
seq_t a(idx_t n)
{
    idx_t r = n + n;          /* r = 2*n (row index) */

    seq_t val = 0;            /* binary number of the row */
    idx_t k = r;              /* start with the full mask */

    /* Enumerate all submasks of r, including 0 */
    while (1) {
        val |= ((seq_t)1) << (r - k);   /* set bit at position (r-k) */
        if (k == 0) break;              /* finished (k==0 handled) */
        k = (k - 1) & r;                /* next submask */
    }

    return (val - 1) / ((seq_t)4);      /* (A001317(2n)-1)/4 */
}
