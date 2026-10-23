/*
 * A128063  Hankel transform of A115962.
 * https://oeis.org/A128063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 30, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128063
   Formula: a(n) = 2^n * Sum_{k=0..⌊(n+2)/2⌋} binomial(n-k+2,k)*(-1)^k*Fibonacci(n-2k+3)
   Recurrence: a(n) = 2*a(n-1) - 4*a(n-2) + 8*a(n-3) - 16*a(n-4)  (n ≥ 4)
   Initial values: a(0)=1, a(1)=2, a(2)=0, a(3)=-8
   The implementation follows the linear recurrence, which holds for all n ≥ 4.
   All arithmetic is performed in type seq_t; no additional libraries or
   floating‑point operations are used.
*/

seq_t a(idx_t n)
{
    /* Handle the first four values directly. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)2;
    if (n == 2u) return (seq_t)0;
    if (n == 3u) return (seq_t)-8;

    /* Keep the last four values of the sequence. */
    seq_t v0 = (seq_t)1;     /* a(n-4)   */
    seq_t v1 = (seq_t)2;     /* a(n-3)   */
    seq_t v2 = (seq_t)0;     /* a(n-2)   */
    seq_t v3 = (seq_t)-8;    /* a(n-1)   */
    seq_t curr;

    /* Iterate from n = 4 up to the requested n. */
    for (idx_t i = 4u; i <= n; ++i) {
        curr = 2 * v3 - 4 * v2 + 8 * v1 - 16 * v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = curr;
    }
    return v3;
}
