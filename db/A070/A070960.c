/*
 * A070960  a(1) = 1; a(n) = n!*(3/2) for n>=2.
 * https://oeis.org/A070960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * (3/2), with a(1) = 1 (defined only for n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;          /* compute n!  (factorial) */
    }
    fact *= (seq_t)3;              /* multiply by 3  */
    fact /= (seq_t)2;              /* divide by 2 – exact for n ≥ 2, gives 1 for n = 1 */
    return fact;
}
