/*
 * A028681  Galois numbers for p=19; order of group AGL(n,19).
 * https://oeis.org/A028681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19^n * Product_{k=0..n-1} (19^n - 19^k)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    seq_t power  = (seq_t)1;           /* current 19^i   */
    for (idx_t i = 1; i <= n; ++i) {
        power = power * (seq_t)19;     /* power = 19^i    */
        seq_t factor = power - (seq_t)1; /* (19^i - 1)   */
        result = result * power * factor; /* multiply by 19^i*(19^i-1) */
    }
    return result;
}
