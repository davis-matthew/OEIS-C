/*
 * A081116  Numbers k such that 17 divides 3^k-k.
 * https://oeis.org/A081116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = base[(n-1)%16] + 272*((n-1)/16) where base =
 *            {5,29,42,75,132,134,140,152,159,162,163,201,206,215,241,256}
 * Range:     u8 n <= 15, u16 n <= 3855, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first 16 solutions modulo 272. */
    static const seq_t base[16] = {
        5, 29, 42, 75,
        132, 134, 140, 152,
        159, 162, 163, 201,
        206, 215, 241, 256
    };

    /* Convert (n-1) once to avoid repeating the subtraction. */
    idx_t k = n - 1u;                /* n >= 1 guarantees k >= 0 */

    /* Determine the block index and residue position. */
    idx_t block   = k / 16u;         /* how many full periods of 16 terms */
    idx_t residue = k % 16u;         /* position within the base array    */

    /* Compute the answer: base + 272 * block. */
    seq_t result = (seq_t)block * (seq_t)272 + base[residue];
    return result;
}
