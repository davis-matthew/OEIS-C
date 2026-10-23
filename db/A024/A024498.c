/*
 * A024498  a(n) = [ C(2n,n)/n ].
 * https://oeis.org/A024498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊C(2n,n)/n⌋, where C is the binomial coefficient
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n ≥ 1.  For n = 0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)          /* safety for offset 1 */
        return 0;

    seq_t result = 1;    /* will hold C(2n,n) */

    /* Compute the central binomial coefficient multiplicatively */
    for (idx_t i = 1; i <= n; ++i) {
        result = (result * (seq_t)(n + i)) / (seq_t)i;
    }

    /* Divide by n to obtain the sequence value */
    return result / (seq_t)n;
}
