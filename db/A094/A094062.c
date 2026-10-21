/*
 * A094062  a(n) = ceiling((3-sqrt(3))*4^(n-3)) + 1 for n>=2, a(1)=1.
 * https://oeis.org/A094062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*4^(n-3) - floor_sqrt(3*16^(n-3)) + 1 for n>=2, a(1)=1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* k = n-3 is the power index for 4^k and 16^k */
    idx_t k = n - 3;

    /* t = floor_sqrt(3*16^i), r = 3*16^i - t^2   (i current index)   */
    seq_t t = (seq_t)1;      /* t_0 = floor_sqrt(3) = 1 */
    seq_t r = (seq_t)2;      /* r_0 = 3 - 1^2 = 2 */
    seq_t power4 = (seq_t)1; /* 4^0 = 1 */

    /* Iterate from i=0 up to i=k-1 to reach t_k */
    while (k > 0) {
        /* u = floor( (4*r) / (2*t + 1) )   = floor(4*(sqrt - t))  */
        seq_t u = (r << 2) / ((t << 1) + (seq_t)1);

        /* Update 4^i */
        power4 = power4 * (seq_t)4;

        /* New t = 4*t + u */
        seq_t t_new = (t << 2) + u;

        /* New r = 16*r - 8*u*t - u^2
           16*r  = r << 4
           8*u*t = (t << 3) * u
        */
        seq_t r_new = (r << 4);
        r_new -= ((t << 3) * u);
        r_new -= (u * u);

        /* Advance to next index */
        t = t_new;
        r = r_new;
        k--;
    }

    /* a(n) = 3*4^(n-3) - t + 1  where t = floor_sqrt(3*16^(n-3)) */
    seq_t result = (seq_t)3 * power4 - t + (seq_t)1;
    return result;
}
