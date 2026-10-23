/*
 * A107955  Number of chains in the power set lattice or the number of fuzzy
 *          subsets of an (n+5)-element set X_(n+5) with specification n
 *          elements of one kind, 4 elements of another and 1 of yet another
 *          kind.
 * https://oeis.org/A107955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(n+1))*(n^5 + 36*n^4 + 431*n^3 + 2088*n^2 + 3972*n +
 *            2304)/24 - 1
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two small cases directly */
    if (n == 0) return (seq_t)191;
    if (n == 1) return (seq_t)1471;

    /* Compute the polynomial P = n^5 + 36*n^4 + 431*n^3 + 2088*n^2 + 3972*n + 2304 */
    seq_t n_seq = (seq_t)n;
    seq_t P = n_seq * n_seq * n_seq * n_seq * n_seq                          /* n^5 */
           + (seq_t)36 * n_seq * n_seq * n_seq * n_seq                       /* 36*n^4 */
           + (seq_t)431 * n_seq * n_seq * n_seq                             /* 431*n^3 */
           + (seq_t)2088 * n_seq * n_seq                                    /* 2088*n^2 */
           + (seq_t)3972 * n_seq                                            /* 3972*n */
           + (seq_t)2304;                                                   /* +2304 */

    /* The entire expression is divisible by 3, so divide first to keep numbers small */
    seq_t P_div3 = P / 3;

    /* Compute 2^(n-2) – use a loop to avoid UB when the shift amount exceeds the bit-width */
    seq_t power_of_two = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i)
        power_of_two <<= 1;   /* safe for all unsigned seq_t types */

    /* Final result: (2^(n-2) * P_div3) - 1  */
    return power_of_two * P_div3 - (seq_t)1;
}
