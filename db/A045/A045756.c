/*
 * A045756  Expansion of e.g.f. (1-9*x)^(-1/9), 9-factorial numbers.
 * https://oeis.org/A045756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=0..n-1} (9*k+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n) = (9*n-8)*a(n-1), n>=1 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        /* multiply by (9*i + 1) */
        result *= (seq_t)(9 * i + 1);
    }
    return result;
}
