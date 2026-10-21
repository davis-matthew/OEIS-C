/*
 * A081346  First column in maze arrangement of natural numbers A081344.
 * https://oeis.org/A081346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 + n + 1 + n*(-1)^n
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = (n+1)^2  if n is even,  a(n) = n^2 + 1  if n is odd  */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;          /* promote index to seq_t for arithmetic */
    if ((n & 1) == 0) {           /* n even => a(n) = (n+1)^2 */
        seq_t np1 = ns + 1;
        return np1 * np1;
    } else {                      /* n odd  => a(n) = n^2 + 1 */
        return ns * ns + (seq_t)1;
    }
}
