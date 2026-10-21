/*
 * A095030  An example of a (v,k,lambda)=(31,6,1) cyclic difference set.
 * https://oeis.org/A095030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A095030 – integers whose residue mod 31 lies in the cyclic
   difference set {1, 5, 11, 24, 25, 27}.  
   Formula:   a(n) = D[(n-1) % 6] + 31 * floor((n-1) / 6),
   where D = {1, 5, 11, 24, 25, 27} and n ≥ 1. */
seq_t a(idx_t n)
{
    static const seq_t D[6] = {1, 5, 11, 24, 25, 27};
    idx_t q = (n - 1) / 6;      /* how many full 31‑blocks precede n */
    idx_t r = (n - 1) % 6;      /* position inside the block          */
    return D[r] + (seq_t)q * 31;
}
