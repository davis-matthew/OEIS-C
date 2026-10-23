/*
 * A117676  Squares for which the digital root is also a square.
 * https://oeis.org/A117676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (9*(n/7) + offset[n%7])^2, where offset = {0,1,2,3,6,7,8}
 * Range:     u8 n <= 11, u16 n <= 199, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The residues modulo 9 that yield a square digital root are
       0,1,2,3,6,7,8.  We map n to the n‑th integer whose residue
       is not 4 or 5, and then square it. */
    static const unsigned int offset[7] = {0,1,2,3,6,7,8};

    seq_t N = (seq_t)n;          /* work in seq_t to avoid overflow */
    seq_t q = N / 7;              /* number of complete blocks of 7 residues */
    idx_t r = n % 7;              /* position within the current block     */
    seq_t m = q * 9 + offset[r];  /* the corresponding integer m          */
    return m * m;                 /* square it                     */
}
