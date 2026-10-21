/*
 * A208138  Number of n X 4 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*(n^2*(n+1)^2)/4
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 158, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn  = (seq_t)n;
    seq_t np1 = (seq_t)(n + 1);

    /* Compute n * (n + 1) */
    seq_t prod = nn * np1;

    /* (n * (n + 1))^2 */
    prod = prod * prod;

    /* 9 * n^2 * (n + 1)^2 */
    prod = prod * 9;

    /* Divide by 4 – guaranteed to be exact because n * (n + 1) is even */
    prod = prod / 4;

    return prod;
}
