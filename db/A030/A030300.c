/*
 * A030300  Runs have lengths 2^n, n >= 0.
 * https://oeis.org/A030300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (1 + (-1)^floor(log_2(n)))/2
 * Range:     u8 n <= 9023, u16 n <= 9023, u32 n <= 9023, u64 n <= 9023,
 *            u128 n <= 9023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    unsigned k = 0;
    idx_t m = n;
    /* Count how many times we can halve m until it becomes 0.
       That's floor(log2(n)).                         */
    while (m >>= 1) {
        ++k;
    }
    /* a(n) = 1 if k is even, else 0. */
    return (seq_t)((k & 1) ? 0 : 1);
}
