/*
 * A060530  Number of inequivalent ways to color edges of a cube using at most
 *          n colors.
 * https://oeis.org/A060530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^12 + 6*n^7 + 3*n^6 + 8*n^4 + 6*n^3)/24
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 52, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate type to avoid overflow when seq_t is 64‑bit. */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;          /* n² */
    unsigned __int128 n3 = n2 * N;         /* n³ */
    unsigned __int128 n4 = n3 * N;         /* n⁴ */
    unsigned __int128 n6 = n4 * n2;         /* n⁶ (n⁴ * n²) */
    unsigned __int128 n7 = n6 * N;          /* n⁷ (n⁶ * n) */
    unsigned __int128 n12 = n6 * n6;        /* n¹² (n⁶ * n⁶) */

    unsigned __int128 num = n12
                          + 6ULL * n7
                          + 3ULL * n6
                          + 8ULL * n4
                          + 6ULL * n3;

    /* Division is exact by the definition of the sequence. */
    seq_t result = (seq_t)(num / 24);
    return result;
}
