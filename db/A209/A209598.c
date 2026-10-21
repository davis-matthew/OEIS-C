/*
 * A209598  Number of 7X7 0..n arrays with every element equal to a diagonal
 *          or antidiagonal reflection.
 * https://oeis.org/A209598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^13 * ((n+1)*(2*n+1))^9
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 13, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The offset is 1; behaviour for n == 0 is irrelevant. */
    if (n == 0) return 0;

    /* Work with n in seq_t to avoid casting at every step. */
    seq_t n_seq = (seq_t)n;
    seq_t np1   = n_seq + 1;                /* n + 1 */

    /* Compute (n+1)^13 */
    seq_t pow13 = 1;
    for (idx_t i = 0; i < 13; ++i)
        pow13 *= np1;

    /* Compute ((n+1)*(2*n+1))^9 */
    seq_t base2 = np1 * (n_seq * 2 + 1);
    seq_t pow9  = 1;
    for (idx_t i = 0; i < 9; ++i)
        pow9 *= base2;

    return pow13 * pow9;
}
