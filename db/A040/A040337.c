/*
 * A040337  Continued fraction for sqrt(356).
 * https://oeis.org/A040337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(356)
   a(0) = 18
   a(n>0) = periodic pattern of length 14: 
   {1,6,1,1,2,1,8,1,2,1,1,6,1,36}
   s.t. a(n) = pattern[(n-1) mod 14]
*/
static const seq_t pattern[14] = {
    (seq_t)1,
    (seq_t)6,
    (seq_t)1,
    (seq_t)1,
    (seq_t)2,
    (seq_t)1,
    (seq_t)8,
    (seq_t)1,
    (seq_t)2,
    (seq_t)1,
    (seq_t)1,
    (seq_t)6,
    (seq_t)1,
    (seq_t)36
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)18;
    idx_t r = (n - 1) % 14;
    return pattern[r];
}
