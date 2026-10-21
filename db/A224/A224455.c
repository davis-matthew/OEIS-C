/*
 * A224455  The hyper-Wiener index of the linear phenylene with n hexagons.
 * https://oeis.org/A224455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/2)*n*(n+1)*(9*n^2 + 3*n + 2)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 132, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */
    seq_t nn = (seq_t)n;           /* use seq_t for all arithmetic */
    seq_t term1 = nn * (nn + 1);   /* n*(n+1) */
    seq_t inner = 9 * nn * nn + 3 * nn + 2; /* 9*n^2 + 3*n + 2 */
    seq_t prod = term1 * inner;    /* (n*(n+1))*(9*n^2+3*n+2) */
    seq_t result = (prod * 3) / 2; /* multiply by 3 then divide by 2 */
    return result;
}
