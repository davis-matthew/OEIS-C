/*
 * A095937  a(n) = Sum_{k=0..n} (k-1)^k.
 * https://oeis.org/A095937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k-1)^k
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute base^exp with exponentiation by squaring.  Works for any seq_t. */
static inline seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term;
        if (k == 0) {
            term = 1;                 /* (-1)^0 = 1 */
        } else {
            seq_t base = (seq_t)(k - 1);
            term = pow_seq_local(base, k);
        }
        sum += term;
    }
    return sum;
}
