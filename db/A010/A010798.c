/*
 * A010798  a(n) = n! * (n+1)! * (n+2)! * (n+3)! / (2! * 3! * 4!).
 * https://oeis.org/A010798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (n+1)! * (n+2)! * (n+3)! / (2! * 3! * 4!) Equivalent
 *            recurrence: a(1) = 1, a(n) = a(n-1) * n * (n+1) * (n+2) * (n+3)
 *            for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1u)
        return 0;              /* below offset – unused */

    seq_t result = 1;          /* a(1) = 1 */

    for (idx_t k = 2u; k <= n; ++k) {
        /* compute the multiplicative factor k*(k+1)*(k+2)*(k+3) */
        seq_t part = (seq_t)k;
        part *= (seq_t)(k + 1u);
        part *= (seq_t)(k + 2u);
        part *= (seq_t)(k + 3u);
        result *= part;
    }

    return result;
}
