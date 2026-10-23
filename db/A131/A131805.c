/*
 * A131805  Row sums of triangular array T: T(j,k) = -(k+1)/2 for odd k,
 *          T(j,k) = 0 for k = 0, T(j,k) = j+1-k/2 for even k > 0; 0 <= k <=
 *          j.
 * https://oeis.org/A131805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor(n/2) * (floor(n/2) + 1) - ceil(n/2)
 * Range:     i8 n <= 23, i16 n <= 363, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where ceil(n/2) = floor(n/2) + (n mod 2)  */
/* This implements the recurrence: a(0)=0; a(n)=a(n-1)+n if n even, a(n)=a(n-1)-1 if n odd */

seq_t a(idx_t n)
{
    /* Compute floor(n/2) without overflow */
    seq_t half = (seq_t)(n >> 1);          /* floor(n/2) */
    seq_t ceil = half + (seq_t)(n & 1);    /* ceil(n/2) = floor(n/2) + (n mod 2) */
    return half * (half + 1) - ceil;
}
