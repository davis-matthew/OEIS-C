/*
 * A223084  Trajectory of 80 under the map n-> A006369(n).
 * https://oeis.org/A223084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=80. For n>1, a(n) = a(n-1) - a(n-1)/3 if a(n-1)%3==0; a(n)
 *            = a(n-1) + a(n-1)/3 if a(n-1)%3==1; a(n) = a(n-1) + a(n-1)/3 + 1
 *            if a(n-1)%3==2
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 217, u64 n <= 1077,
 *            u128 n <= 1730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 80;                     /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        seq_t q = x / 3;              /* floor division by 3 */
        seq_t r = x % 3;              /* remainder 0,1,2 */

        if (r == 0) {
            x = x - q;                /* subtract quotient */
        } else if (r == 1) {
            x = x + q;                /* add quotient */
        } else { /* r == 2 */
            x = x + q + 1;            /* add quotient + 1 */
        }
    }
    return x;
}
