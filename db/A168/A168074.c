/*
 * A168074  Duplicate of A061891.
 * https://oeis.org/A168074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 + 6*floor(n/3) + (n%3 == 2 ? 3 : 0)
 * Range:     i8 n <= 64, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Integer division and remainder are performed on idx_t (uint64_t). */
    idx_t q = n / 3;          /* floor(n/3) */
    idx_t r = n % 3;          /* n mod 3 */
    
    /* Convert the integer parts to seq_t and compute the result. */
    seq_t val = (seq_t)1 + (seq_t)q * (seq_t)6;
    if (r == 2)
        val += (seq_t)3;
    return val;
}
