/*
 * A064537  Limit of the recursion B_[k] = P[k](B_[k-1]), where B_[0] =
 *          (1,2,3,4,5,...) and P[k] is the permutation that permutes the
 *          entries k+j and 2k + j for all j = 1,..,k.
 * https://oeis.org/A064537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the value that ends up in position n after repeatedly
 * applying, for k = 1,2,3,…, the permutation that swaps positions
 * k + j and 2k + j for j = 1..k, starting from the identity sequence.
 * The permutation is applied for all k < n, and the image of n under
 * the composed permutation  P[n-1]∘…∘P[1] gives a(n).
 * This function computes that image by walking the swaps in reverse
 * order (k = n−1 … 1).  It uses only integer arithmetic and works
 * for any seq_t size. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; ignore n=0 */

    seq_t pos = (seq_t)n;                 /* current position/value */

    for (idx_t k = n - 1; k >= 1; --k) {
        seq_t k1 = (seq_t)(k + 1);         /* start of first block   */
        seq_t k2 = (seq_t)(k * 2);         /* end   of first block   */
        seq_t k3 = k2 + (seq_t)k;          /* end   of second block  */

        if (pos >= k1 && pos <= k2) {     /* inside first block */
            pos += (seq_t)k;
        } else if (pos >= k2 + (seq_t)1 && pos <= k3) { /* second block */
            pos -= (seq_t)k;
        }
        /* otherwise pos unchanged */
    }

    return pos;
}
