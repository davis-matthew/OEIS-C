/*
 * A057063  Let P(n) of a sequence s(1),s(2),s(3),... be obtained by leaving
 *          s(1),...,s(n) fixed and reverse-cyclically permuting every n
 *          consecutive terms thereafter; apply P(2) to 1,2,3,... to get
 *          PS(2), then apply P(3) to PS(2) to get PS(3), then apply P(4) to
 *          PS(3), etc. The limit of PS(n) is A057063.
 * https://oeis.org/A057063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A057063
 * Conjecture: a(n) = A057033(n-1) + 1  (for n > 1, a(1) = 1)
 * 
 * The sequence is obtained by starting with the infinite
 * identity permutation 1,2,3,... and successively applying,
 * for k = 2,3,..., the permutation P(k):  
 *   leave the first k terms fixed, then for each block of k consecutive
 *   terms starting at position k+1, rotate that block left by one
 *   position (e.g. 3 6 5 → 6 5 3).  After all permutations have been
 *   applied, the item that ends up in position n is a(n).  For a fixed
 *   n, permutations with k ≥ n leave the first n positions unchanged,
 *   so it suffices to apply P(k) for 2 ≤ k < n.  The algorithm below
 *   builds an array large enough to contain all numbers that can be
 *   moved into the first n positions during these steps and returns
 *   the value at index n.
 */

#include <stdlib.h>

/* compute the nth term of OEIS sequence A057063 */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* a maximum index that could influence the first n positions
     * is less than 2*n (e.g. for k = n-1 the block spans up to 2n-2).
     * Allocating 2*n + 1 elements with 1‑based indexing covers all cases. */
    idx_t M = n + n;                     /* 2*n */
    seq_t *seq = (seq_t *)malloc((M + 1) * sizeof(seq_t));
    if (!seq)
        return (seq_t)0;                 /* allocation failed */

    /* initialise with the identity permutation */
    for (idx_t i = (idx_t)1; i <= M; ++i)
        seq[i] = (seq_t)i;

    /* apply P(k) for k = 2 .. n-1 */
    for (idx_t k = (idx_t)2; k < n; ++k) {
        /* start positions of blocks: k+1, k+1+k, k+1+2k, ... */
        for (idx_t start = k + (idx_t)1;
             start + k - (idx_t)1 <= M; start += k)
        {
            seq_t tmp = seq[start];
            /* shift left by one: seq[start] = seq[start+1], etc. */
            for (idx_t j = start; j + (idx_t)1 <= start + k - (idx_t)1; ++j)
                seq[j] = seq[j + (idx_t)1];
            seq[start + k - (idx_t)1] = tmp;
        }
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
