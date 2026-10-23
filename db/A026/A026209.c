/*
 * A026209  Duplicate of A026185.
 * https://oeis.org/A026209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2*n for even n a(n) = 4*floor(((n+1)/2 - 1)/3) +
 *            (((n+1)/2 - 1)%3) + 1 for odd n
 * Range:     i8 n <= 61, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        /* even n: a(n) = 2*n */
        return (seq_t)n * 2;
    } else {
        /* odd n: compute the ((n+1)/2)-th positive integer not divisible by 4 */
        seq_t s = ((seq_t)n + 1) / 2;        /* position among odd indices */
        seq_t t = s - 1;                     /* zero‑based rank among these numbers */
        seq_t q = t / 3;                     /* how many full blocks of 3 numbers */
        seq_t r = t % 3;                     /* offset within the current block */
        return 4 * q + r + 1;
    }
}
