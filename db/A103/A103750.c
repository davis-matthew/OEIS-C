/*
 * A103750  Expansion of (1+2*x^3)/(1-x+x^3-2*x^4).
 * https://oeis.org/A103750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = a(n-1) - a(n-3) + 2*a(n-4) a(0)=1, a(1)=1, a(2)=1, a(3)=2
 * Range:     u8 n <= 21, u16 n <= 41, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t init[4] = {1, 1, 1, 2};

    if (n < 4)
        return init[n];

    /* Variables holding a(n-4), a(n-3), a(n-2), a(n-1) respectively */
    seq_t v0 = init[0];   /* a(n-4) */
    seq_t v1 = init[1];   /* a(n-3) */
    seq_t v2 = init[2];   /* a(n-2) */
    seq_t v3 = init[3];   /* a(n-1) */

    seq_t next;
    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute a(i) = v3 - v1 + 2*v0 */
        seq_t two_v0 = v0 + v0;
        next = v3 - v1 + two_v0;

        /* Shift the window forward by one */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }
    return v3;
}
