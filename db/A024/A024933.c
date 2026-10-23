/*
 * A024933  a(n) = Sum_{k=1..n} k*[ (n/k)*[ (n/k)*[ n/k ] ] ].
 * https://oeis.org/A024933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} k * floor( (n * floor( (n * floor(n/k)) / k ))
 *            / k )
 * Range:     u8 n <= 5, u16 n <= 34, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* floor(n/k) */
        seq_t f1 = n_seq / k;

        /* floor((n * f1) / k) = floor((n/k) * f1) */
        seq_t f2 = (n_seq * f1) / k;

        /* floor((n * f2) / k) = floor((n/k) * f2) */
        seq_t f3 = (n_seq * f2) / k;

        /* add k * f3 to the sum */
        sum += (seq_t)k * f3;
    }

    return sum;
}
