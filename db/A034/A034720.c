/*
 * A034720  Number of different words that can be formed from an n X n grid of
 *          letters, reading horizontally, vertically or diagonally.
 * https://oeis.org/A034720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2n-1)*(5n-2)/3
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n<1, return 0 as guard */
    seq_t n0 = (seq_t)n;           /* cast to seq_t for safe arithmetic   */
    seq_t term1 = 2 * n0 - 1;      /* (2n-1)                              */
    seq_t term2 = 5 * n0 - 2;      /* (5n-2)                              */
    seq_t prod = n0 * term1;       /* n*(2n-1) */
    prod *= term2;                 /* n*(2n-1)*(5n-2) */
    return prod / 3;               /* division is exact for all n>=1       */
}
