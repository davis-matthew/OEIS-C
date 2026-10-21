/*
 * A207165  Number of n X 4 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*n^3 + 9*n - 9
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before any arithmetic to keep
       everything in the desired type. */
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;     /* n^2 */
    seq_t n3 = n2 * n_seq;        /* n^3 */
    return 9 * n3 + 9 * n_seq - 9;
}
