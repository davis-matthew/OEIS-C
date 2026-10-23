/*
 * A159888  Numbers congruent to
 *          {-5,29,39,41,43,45,55,57,59,93,103,105,107,109,119,121} mod 256.
 * https://oeis.org/A159888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer whose residue modulo 256 belongs to
 *            {29,39,41,43,45,55,57,59,93,103,105,107,109,119,121,251}
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence of allowed residues sorted increasingly */
    static const seq_t R[16] =
    {
        29, 39, 41, 43, 45, 55, 57, 59,
        93,103,105,107,109,119,121,251
    };

    if (n == 0) /* offset is 1; n should never be 0 but guard anyway */
        return 0;

    idx_t pos = n - 1;          /* zero‑based index */
    idx_t q   = pos / 16;       /* number of completed periods of 256 */
    idx_t r   = pos % 16;       /* index into R for this period */

    /* Convert to seq_t before multiplication to avoid overflow in small types */
    return R[r] + (seq_t)q * (seq_t)256;
}
