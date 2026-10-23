/*
 * A033183  a(n) = number of pairs (p,q) such that 4*p + 9*q = n.
 * https://oeis.org/A033183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of solutions to 4p + 9q = n = { ((floor(n/9) -
 *            n%4) / 4) + 1, if floor(n/9) ≥ n%4 0, otherwise } "count
 *            solutions to the linear Diophantine equation"
 * Range:     u8 n <= 9179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* maximum number of 9‑coin parts */
    idx_t maxq = n / (idx_t)9;
    /* the residue of n modulo 4 determines the admissible q */
    idx_t rem  = n % (idx_t)4;

    if (maxq < rem)
        return (seq_t)0;

    /* number of possible q values is ((maxq - rem) / 4) + 1 */
    seq_t count = (seq_t)((maxq - rem) / (idx_t)4) + (seq_t)1;
    return count;
}
