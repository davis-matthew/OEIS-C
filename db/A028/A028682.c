/*
 * A028682  Pseudo Galois numbers for d=20.
 * https://oeis.org/A028682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20^n * Π_{k=0}^{n-1} (20^n - 20^k)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* a(0) = 1 */
        return (seq_t)1;

    seq_t result = (seq_t)1;
    seq_t pow = (seq_t)20;          /* pow = 20^i on iteration i */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = pow - (seq_t)1;        /* 20^i - 1        */
        result = result * term;             /* multiply by term */
        result = result * pow;              /* accumulate 20^sum */
        pow = pow * (seq_t)20;              /* prepare next power 20^{i+1} */
    }

    return result;
}
