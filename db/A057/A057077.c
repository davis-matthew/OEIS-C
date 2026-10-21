/*
 * A057077  Periodic sequence 1,1,-1,-1; expansion of (1+x)/(1+x^2).
 * https://oeis.org/A057077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(floor(n/2)) = 1 if n mod 4 ∈ {0,1}, otherwise -1.
 *            An O(1) implementation using a bit test
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((n & 2) ? (seq_t)-1 : (seq_t)1);
}
