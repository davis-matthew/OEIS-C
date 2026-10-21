/*
 * A055462  Superduperfactorials: product of first n superfactorials.
 * https://oeis.org/A055462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1}^n A000178(i), where a(0) = 1 Recurrence:
 *            a(0)=1, a(n)=a(n-1)*A000178(n) A000178(n) = superfactorial =
 *            Product_{k=1..n} k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handles n==0 correctly */
    if (n == 0)
        return (seq_t)1;

    /* result = a(n-1), superfactorial = sf(n) = product_{k=1..n} k!,
       factorial = n!     */
    seq_t result = 1;
    seq_t superfactorial = 1;
    seq_t factorial = 1;

    for (idx_t i = 1; i <= n; ++i) {
        /* factorial = i! */
        factorial *= (seq_t)i;

        /* superfactorial = sf(i) = sf(i-1) * i! */
        superfactorial *= factorial;

        /* sequence value = previous a * superfactorial */
        result *= superfactorial;
    }

    return result;
}
