/*
 * A235011  Number of (n+1) X (2+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) - 14*a(n-3) - 4*a(n-4) + 16*a(n-5)
 *            Initial values: a(1)=104, a(2)=224, a(3)=488, a(4)=1112,
 *            a(5)=2568
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 20, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but return 0 */

    /* Initial terms for n = 1..5 */
    static const seq_t init[5] = {104, 224, 488, 1112, 2568};

    if (n <= 5) return init[n-1];

    /* Sequence sliding window: t0=a(n-5), t1=a(n-4), … t4=a(n-1) */
    seq_t t0 = init[0];   /* a(1)  */
    seq_t t1 = init[1];   /* a(2)  */
    seq_t t2 = init[2];   /* a(3)  */
    seq_t t3 = init[3];   /* a(4)  */
    seq_t t4 = init[4];   /* a(5)  */

    seq_t new_val;

    for (idx_t i = 6; i <= n; ++i)
    {
        /* a(i) = 3*a(i-1) + 4*a(i-2) - 14*a(i-3) - 4*a(i-4) + 16*a(i-5) */
        new_val = (seq_t)3 * t4
                + (seq_t)4 * t3
                - (seq_t)14 * t2
                - (seq_t)4 * t1
                + (seq_t)16 * t0;

        /* Shift window for next iteration */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = new_val;
    }

    return new_val;   /* a(n) */
}
