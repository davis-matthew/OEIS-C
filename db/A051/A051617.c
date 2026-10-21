/*
 * A051617  a(n) = (4*n+5)(!^4)/5(!^4), related to A007696(n+1) ((4*n+1)(!^4)
 *          quartic, or 4-factorials).
 * https://oeis.org/A051617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A051617
 * Formula: a(n) = ∏_{k=1}^{n} (4k + 5)
 * Recurrence: a(0) = 1,  a(n) = a(n-1) * (4n + 5)  for n ≥ 1
 */
seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t factor = ( (seq_t)4 * (seq_t)k ) + 5;
        result *= factor;
    }
    return result;
}
