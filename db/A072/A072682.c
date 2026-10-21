/*
 * A072682  Numbers congruent to {3, 36, 54, 57} mod 60.
 * https://oeis.org/A072682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = residues[(n-1) mod 4] + 60*floor((n-1)/4) Residues are
 *            {3, 36, 54, 57} modulo 60
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t residues[4] = { (seq_t)3, (seq_t)36, (seq_t)54, (seq_t)57 };

seq_t a(idx_t n)
{
    /* n >= 1 by definition.  For safety, handle n==0. */
    if (n == 0) return (seq_t)0;
    idx_t idx = n - 1;          /* zero‑based index */
    idx_t q   = idx / 4;         /* block number (each block of 4 numbers) */
    idx_t r   = idx % 4;         /* position inside the block */
    return residues[r] + ((seq_t)60 * (seq_t)q);
}
