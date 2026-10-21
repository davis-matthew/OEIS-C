/*
 * A060852  Sum of number of pips on dice in shape of Platonic solids.
 * https://oeis.org/A060852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = F[n-1]*(F[n-1]+1)/2, where F={4,6,8,12,20} for the five
 *            Platonic solids. For n > 5 the sequence is undefined, but we
 *            return 0
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const idx_t platonic_faces[5] = {4, 6, 8, 12, 20};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset is 1, but guard against n==0 */

    if (n <= 5) {
        seq_t f = platonic_faces[n - 1]; /* number of faces of the n‑th Platonic solid */
        return f * (f + 1) / 2;          /* sum of numbers 1..f on each face */
    }
    return 0;                          /* sequence is finite */
}
