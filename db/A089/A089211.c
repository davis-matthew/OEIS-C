/*
 * A089211  a(n) = 10*n^n+(n-1).
 * https://oeis.org/A089211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*n^n + (n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq(idx_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1U)
            result *= b;
        if (exp > 1U)
            b *= b;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined offset, return 0 */
    seq_t power = pow_seq(n, n); /* compute n^n */
    power *= 10;                 /* multiply by 10 */
    power += (seq_t)(n - 1);     /* add (n-1) */
    return power;
}
