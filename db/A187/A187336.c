/*
 * A187336  Sum{floor(kn/6), k=1,2,3,4,5}.
 * https://oeis.org/A187336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum{floor(k*n/6), k=1..5}
 * Range:     u8 n <= 103, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rn = (seq_t)n;
    seq_t sum = rn / 6;                 /* floor(n/6)          */
    sum += rn / 3;                      /* floor(2n/6) = n/3   */
    sum += rn / 2;                      /* floor(3n/6) = n/2   */
    seq_t ceil3 = rn / 3 + ((rn % 3) != 0);
    sum += rn - ceil3;                  /* floor(4n/6) = 2n/3  */
    seq_t ceil6 = rn / 6 + ((rn % 6) != 0);
    sum += rn - ceil6;                  /* floor(5n/6)        */
    return sum;
}
