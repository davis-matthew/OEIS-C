/*
 * A175868  Partial sums of ceiling(n^2/20).
 * https://oeis.org/A175868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 + 70*n + 72)/120)
 * Range:     u8 n <= 23, u16 n <= 157, u32 n <= 6363, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for integral seq_t types to avoid overflow */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 N = n;
        unsigned __int128 num = 2 * N * N * N
                              + 3 * N * N
                              + 70 * N
                              + 72;
        return (seq_t)(num / 120);
    } else {
        seq_t N = n;                          /* N is already a big integer type */
        seq_t num = 2 * N * N * N
                  + 3 * N * N
                  + 70 * N
                  + 72;
        return num / 120;
    }
}
