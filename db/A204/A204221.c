/*
 * A204221  Integers of the form (N^2 - 1) / 15.
 * https://oeis.org/A204221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((15 * floor(n/4) + t[n mod 4])^2 - 1) / 15 where t = {1,
 *            4, 11, 14}
 * Range:     u8 n <= 3, u16 n <= 140, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the block and position within the block */
    idx_t k = n / 4;          /* floor division by 4 */
    idx_t r = n % 4;          /* remainder 0..3 */

    /* Offsets for each residue class modulo 15 */
    static const seq_t offset[4] = {
        (seq_t)1,  /* for r = 0 → N ≡ 1 (mod 15) */
        (seq_t)4,  /* for r = 1 → N ≡ 4 (mod 15) */
        (seq_t)11, /* for r = 2 → N ≡ 11 (mod 15) */
        (seq_t)14  /* for r = 3 → N ≡ 14 (mod 15) */
    };

    /* Compute N = 15*k + offset[r] */
    seq_t N = (seq_t)15 * (seq_t)k + offset[r];

    /* a(n) = (N^2 - 1) / 15 */
    seq_t sq = N * N;
    seq_t result = (sq - (seq_t)1) / (seq_t)15;

    return result;
}
