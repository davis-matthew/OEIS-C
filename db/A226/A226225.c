/*
 * A226225  Expansion of phi(q) * phi(q^8) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A226225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the weighted count of representations * n = k^2 + 8·l^2
 *            with integers k,l. For each solution the weight is * (k==0?1:2)
 *            * (l==0?1:2) so that zero contributes a single term, * non‑zero
 *            contributions are counted twice for their positive and *
 *            negative signs. This matches the generating func...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* l ranges over non‑negative integers satisfying 8 * l^2 ≤ n */
    for (idx_t l = 0; 8 * l * l <= n; ++l)
    {
        idx_t rhs = n - 8 * l * l;          /* remaining to be a square */
        /* try all k with k^2 ≤ rhs */
        for (idx_t k = 0; k * k <= rhs; ++k)
        {
            if (k * k == rhs)
            {
                /* weight = (k==0?1:2) * (l==0?1:2) */
                seq_t weight = (seq_t)((k==0 ? 1 : 2) * (l==0 ? 1 : 2));
                sum += weight;
                break;                       /* exact square found, go to next l */
            }
        }
    }
    return sum;
}
