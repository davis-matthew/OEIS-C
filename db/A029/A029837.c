/*
 * A029837  Binary order of n: log_2(n) rounded up to next integer.
 * https://oeis.org/A029837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ceil(log_2(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0; for n > 1, a(n) = floor(log_2(n-1)) + 1 */

/* Computes the binary order of n: the number of bits required to
 * represent (n-1) in binary, which is equal to ceil(log2(n)). */
seq_t a(idx_t n)
{
    /* n is at least 1 */
    idx_t m = n - 1;          /* n-1 fits in idx_t (uint64_t) */
    seq_t k = 0;              /* result accumulator */

    /* Count the number of bits needed to represent m */
    while (m > 0) {
        m >>= 1;
        k++;
    }

    /* For n == 1, m == 0 and k == 0, which is correct */
    return k;
}
