/*
 * A095038  An example of a (v,k,lambda)=(133,12,1) cyclic difference set.
 * https://oeis.org/A095038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = S[(n-1) mod 12] + 133 * floor((n-1) / 12) where S =
 *            {1,10,11,13,27,31,68,75,83,110,115,121} are the elements of the
 *            cyclic (133,12,1) difference set
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The base residues of the difference set (sorted ascending). */
    static const seq_t S[12] = {1, 10, 11, 13, 27, 31, 68, 75, 83, 110, 115, 121};

    /* Zero‑based position within the block of 12 elements. */
    const idx_t pos   = (n - 1) % 12;
    /* How many full blocks of 12 precede the current one. */
    const idx_t block = (n - 1) / 12;

    /* Build the answer: base residue + 133 times the block number. */
    return S[pos] + (seq_t)133 * (seq_t)block;
}
