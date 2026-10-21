/*
 * A205842  Numbers k for which 3 divides s(k)-s(j) for some j<k; each k
 *          occurs once for each such j; s(k) denotes the (k+1)-st Fibonacci
 *          number.
 * https://oeis.org/A205842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th element of the sequence defined by
   the pairs (k,j) with 0 < j < k and s(k) ≡ s(j) (mod 3), where
   s(k) is the (k+1)‑st Fibonacci number.  Each k is output once for
   each such j.  The algorithm iterates k upward, counts the number of
   earlier indices with the same residue modulo 3, and accumulates the
   total number of occurrences until it reaches n. */

seq_t a(idx_t n)
{
    /* Fibonacci residues modulo 3 for s(k)=F_{k+1}, k=1..∞.
       The pattern repeats every 8 elements: 1,2,0,2,2,1,0,1.
       We use 0,1,2 as indices for the residue counts. */
    static const int pattern[8] = {1, 2, 0, 2, 2, 1, 0, 1};

    /* Count of indices seen so far with each residue (0,1,2). */
    int res_counts[3] = {0, 0, 0};

    idx_t k = 1;          /* current index k */
    idx_t cum = 0;        /* cumulative number of occurrences produced so far */

    while (1) {
        int r = pattern[(k - 1) & 7U];       /* residue of s(k) modulo 3 */
        idx_t cnt = (idx_t)res_counts[r];    /* earlier j with same residue */

        cum += cnt;
        if (cum >= n)
            return (seq_t)k;                /* found the n-th occurrence */

        /* Record this k for future counts. */
        res_counts[r]++;

        k++;
    }
}
