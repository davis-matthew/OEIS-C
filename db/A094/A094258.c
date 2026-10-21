/*
 * A094258  a(1) = 1, a(n+1) = n*n! for n >= 1.
 * https://oeis.org/A094258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, for n >= 2: a(n) = n! – (n-1)! = (n-1)*(n-1)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;

    /* Compute (n-1)! */
    for (idx_t i = 2; i < n; ++i) {
        /* multiply result by i, cast i to seq_t first */
        result *= (seq_t)i;
    }

    /* multiply by (n-1) to get (n-1)*(n-1)! = n! - (n-1)! */
    result *= (seq_t)(n - 1);

    return result;
}
