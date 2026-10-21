/*
 * A092418  A sieve: starting with the sequence of positive integers, delete
 *          every 4th number, then delete every 16th number from the remaining
 *          sequence, then delete every 64th number, etc. Sequence gives the
 *          remaining numbers.
 * https://oeis.org/A092418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = value obtained by undoing the sieve deletions: starting
 *            with index n, for k from the largest 4^k ≤ n down to 1, replace
 *            index j by j + floor((j-1)/(4^k-1)). The resulting integer
 *            equals the a(n) term
 * Range:     u8 n <= 178, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t j = (seq_t)n;            /* current index after all deletions */
    seq_t pow4 = (seq_t)1;         /* 4^k */
    int maxK = 0;
    /* Find the largest k with 4^k ≤ n, i.e. 4^(k+1) > n.
       Use division to avoid overflow. */
    while (pow4 <= n / (seq_t)4) {
        pow4 *= (seq_t)4;
        ++maxK;
    }

    /* Process steps from k = maxK down to 1.
       For each k, the block size of kept elements is 4^k - 1. */
    for (int k = maxK; k >= 1; --k) {
        seq_t block = pow4 - (seq_t)1;
        j += (j - (seq_t)1) / block;
        pow4 /= (seq_t)4;        /* move to next, smaller power of 4 */
    }

    /* After all inverses, j is the original number in the
       initial sequence 1,2,3,... which is a(n). */
    return j;
}
