/*
 * A047343  Numbers that are congruent to {1, 3, 4} mod 7.
 * https://oeis.org/A047343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * floor((n-1)/3) + {1,3,4} where the offset starts at n
 *            = 1
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1 */
    idx_t k = (n - 1) / 3;           /* floor((n-1)/3) */
    idx_t r = (n - 1) % 3;           /* remainder 0,1,2 */

    seq_t kk = (seq_t)k;             /* cast index to seq_t for multiplication */

    switch (r) {
    case 0: return kk * 7 + 1;       /* a(3k+1)  = 7k + 1  */
    case 1: return kk * 7 + 3;       /* a(3k+2)  = 7k + 3  */
    default: return kk * 7 + 4;      /* a(3k+3)  = 7k + 4  */
    }
}
