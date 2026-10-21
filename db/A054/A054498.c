/*
 * A054498  Number of symmetric nonnegative integer 8 X 8 matrices with sum of
 *          elements equal to 4*n, under action of dihedral group D_4.
 * https://oeis.org/A054498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((8+n)*(2835*(1739+309*(-1)^n) + *
 *            576*(15259+1029*(-1)^n)*n + * 36*(166171+3717*(-1)^n)*n^2 + *
 *            448*(4661+27*(-1)^n)*n^3 + * 14*(29749+27*(-1)^n)*n^4 + *
 *            49280*n^5 + 3416*n^6 + 128*n^7 + 2*n^8)) / 46448640
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 69, u64 n <= 902,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow while still
       allowing a 64‑bit final result for small n. */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;
    unsigned __int128 n5 = n4 * N;
    unsigned __int128 n6 = n5 * N;
    unsigned __int128 n7 = n6 * N;
    unsigned __int128 n8 = n7 * N;

    /* (-1)^n as +1 or -1 */
    int64_t s = (n & 1ULL) ? -1 : 1;

    /* integer constants from the formula */
    int64_t inner1_64 = 1739 + 309 * s;          /* 2048 or 1430 */
    int64_t inner2_64 = 15259 + 1029 * s;        /* 16288 or 14230 */
    int64_t inner3_64 = 166171 + 3717 * s;       /* 169888 or 162454 */
    int64_t inner4_64 = 4661 + 27 * s;           /* 4688 or 4634 */
    int64_t inner5_64 = 29749 + 27 * s;          /* 29776 or 29722 */

    unsigned __int128 term1 = (unsigned __int128)2835 *
                              (unsigned __int128)inner1_64;
    unsigned __int128 term2 = (unsigned __int128)576 *
                              (unsigned __int128)inner2_64 * N;
    unsigned __int128 term3 = (unsigned __int128)36 *
                              (unsigned __int128)inner3_64 * n2;
    unsigned __int128 term4 = (unsigned __int128)448 *
                              (unsigned __int128)inner4_64 * n3;
    unsigned __int128 term5 = (unsigned __int128)14 *
                              (unsigned __int128)inner5_64 * n4;
    unsigned __int128 sum   = term1 + term2 + term3 + term4 + term5
                               + (unsigned __int128)49280 * n5
                               + (unsigned __int128)3416  * n6
                               + (unsigned __int128)128   * n7
                               + (unsigned __int128)2     * n8;

    unsigned __int128 num = (N + (unsigned __int128)8) * sum;
    uint64_t den = 46448640ULL;

    unsigned __int128 res128 = num / den;
    return (seq_t)res128;
}
