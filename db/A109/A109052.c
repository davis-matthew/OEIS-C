/*
 * A109052  a(n) = lcm(n,11).
 * https://oeis.org/A109052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*11/gcd(n,11)
 * Range:     u8 n <= 23, u16 n <= 5957, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* cast index to the value type   */
    seq_t one_eleven = (seq_t)11;    /* constant 11 as seq_t             */

    /* Compute gcd(n, 11) via the Euclidean algorithm. */
    seq_t A = n_seq;                 /* operand a                     */
    seq_t B = one_eleven;            /* operand b                     */
    while (B != (seq_t)0) {
        seq_t r = A % B;
        A = B;
        B = r;
    }
    seq_t g = A;                     /* g == gcd(n,11)                 */

    /* Avoid overflow as much as possible: multiply
       after division by the gcd.  Since gcd is 1 or 11,
       this is safe for all n fitting in seq_t.        */
    seq_t result = (n_seq / g) * one_eleven;
    return result;
}
