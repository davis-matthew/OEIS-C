/*
 * A084939  Pentagorials: n-th polygorial for k=5.
 * https://oeis.org/A084939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(n+1) = ((n+1)*(3*n+2)/2) * a(n) (i.e. a(n) =
 *            product_{k=0}^{n-1} ((k+1)*(3*k+2)/2) ) The recurrence only uses
 *            integer arithmetic and works for any seq_t that supports +, -,
 *            *, /, % and unary casts
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 19,
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
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t mul = (seq_t)(k + 1) * (seq_t)(3 * k + 2);
        mul /= 2;                /* guaranteed to be integral */
        result = result * mul;   /* exact multiplication in seq_t */
    }
    return result;
}
