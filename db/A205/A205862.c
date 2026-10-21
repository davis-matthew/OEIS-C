/*
 * A205862  Numbers k for which 7 divides s(k)-s(j) for some j<k; each k
 *          occurs once for each such j; s(k) denotes the (k+1)-st Fibonacci
 *          number.
 * https://oeis.org/A205862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k such that cumulative = Σ_{s∈{0..6}}
 *            C(counts[s],2) ≥ n where counts[s] is the number of indices ≤k
 *            having residue s
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the index k of the Fibonacci number s(k)=F_{k+1} for which there are exactly n
   pairs (j,k') with j<k'≤k and s(j)≡s(k') (mod 7).  Each such pair contributes
   one occurrence of its larger index k in the sequence.  The algorithm enumerates
   k, keeps a count of how many of the previous indices have each residue modulo 7,
   and sums these counts until the cumulative total reaches n.  */


seq_t a(idx_t n)
{
    /* residues of s(k)=F_{k+1} modulo 7 for k=1..16 (period 16) */
    static const unsigned char residues[16] =
        {1,2,3,5,1,6,0,6,6,5,4,2,6,1,0,1};

    idx_t counts[7] = {0};          /* counts of each residue seen so far   */
    idx_t cumulative = 0;           /* number of pairs constructed so far   */
    idx_t k = 1;                    /* current index to test               */

    while (cumulative < n)
    {
        unsigned int residue = residues[(k-1) & 15];
        idx_t m = counts[residue];          /* multiplicity of current k */
        cumulative += m;
        if (cumulative >= n)                /* found the n-th entry */
            return (seq_t)k;
        counts[residue] = m + 1;             /* record current k  */
        ++k;
    }
    return (seq_t)k;    /* should never be reached */
}
