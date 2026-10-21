/*
 * A218155  Numbers congruent to 2, 3, 6, 11 mod 12.
 * https://oeis.org/A218155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A218155: numbers congruent to 2, 3, 6, or 11 modulo 12. */
/* a(n) = 12 * floor((n‑1)/4) + residue[(n‑1) mod 4] where residue = {2,3,6,11} */
seq_t a(idx_t n)
{
    /* Zero‑based index blocks and remainder.  All arithmetic is done in idx_t
       before converting to seq_t for the final result. */
    idx_t q = (n - 1) / 4;      /* number of full 12‑length blocks   */
    idx_t r = (n - 1) % 4;      /* position within the block         */

    /* The four allowed residues in the required order. */
    static const seq_t residues[4] = { (seq_t)2, (seq_t)3, (seq_t)6, (seq_t)11 };

    /* Construct the n‑th term.  Cast to seq_t before multiplication to
       avoid overflow in the intermediate product. */
    return (seq_t)q * (seq_t)12 + residues[(unsigned int)r];
}
