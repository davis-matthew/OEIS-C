/*
 * A129725  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+521)^2 = y^2.
 * https://oeis.org/A129725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 1042; a(1)=0, a(2)=100, a(3)=1159,
 *            a(4)=1563, a(5)=2079, a(6)=8080
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial six terms */
    static const seq_t init[6] =
    {
        0,      /* a(1) */
        100,    /* a(2) */
        1159,   /* a(3) */
        1563,   /* a(4) */
        2079,   /* a(5) */
        8080    /* a(6) */
    };

    if (n <= 6)
        return init[n - 1];

    /* Circular buffer holding the last six computed terms */
    seq_t buf[6];
    idx_t i;

    /* Load the initial terms into the buffer */
    for (i = 1; i <= 6; ++i)
        buf[(i - 1) % 6] = init[i - 1];

    /* Compute successive terms up to the requested index n */
    for (i = 7; i <= n; ++i)
    {
        seq_t val = 6 * buf[(i - 3 - 1) % 6]       /* a(i-3)   */
                  - buf[(i - 6 - 1) % 6] + 1042;   /* a(i-6)   */
        buf[(i - 1) % 6] = val;
    }

    return buf[(n - 1) % 6];
}
