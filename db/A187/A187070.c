/*
 * A187070  Let i be in {1,2,3}, let r >= 0 be an integer and n=2*r+i-1. Then
 *          a(n)=a(2*r+i-1) gives the quantity of H_(7,3,0) tiles in a
 *          subdivided H_(7,i,r) tile after linear scaling by the factor x^r,
 *          where x=sqrt((2*cos(Pi/7))^2-1).
 * https://oeis.org/A187070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-4) - a(n-6); Initial values: a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=2
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 58, u64 n <= 112,
 *            u128 n <= 222, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* A small circular buffer of size 7 to hold the last 7 values.
       Indexing modulo 7 is sufficient because the recurrence uses steps of 2. */
    seq_t buf[7] = {0};

    /* Initialise the base cases up to index 5 (enable for all n). */
    if (n >= 0) buf[0] = 0;
    if (n >= 1) buf[1] = 0;
    if (n >= 2) buf[2] = 1;
    if (n >= 3) buf[3] = 1;
    if (n >= 4) buf[4] = 1;
    if (n >= 5) buf[5] = 2;
    /* buf[6] remains 0 until it is filled by the loop. */

    if (n <= 5)
        return buf[n];

    /* Compute values from 6 upwards. */
    for (idx_t i = 6; i <= n; ++i) {
        idx_t idx = i % 7;
        idx_t i2  = (i + 5) % 7;   /* i-2 mod 7 */
        idx_t i4  = (i + 3) % 7;   /* i-4 mod 7 */
        idx_t i6  = (i + 1) % 7;   /* i-6 mod 7 */
        buf[idx] = 2 * buf[i2] + buf[i4] - buf[i6];
    }

    return buf[n % 7];
}
