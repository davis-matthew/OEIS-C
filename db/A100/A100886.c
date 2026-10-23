/*
 * A100886  Expansion of x*(1+3*x+2*x^2)/((1+x+x^2)*(1-x-x^2)).
 * https://oeis.org/A100886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + a(n-4) (n ≥ 4) a(0) = 0, a(1) = 1,
 *            a(2) = 3, a(3) = 3
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms for indices 0..3 */
    static const seq_t init[4] = {0, 1, 3, 3};

    if (n < 4)
        return init[n];

    /* Circular buffer holding the last four computed values:
       positions correspond to indices:
       buf[0] = a(k-4), buf[1] = a(k-3), buf[2] = a(k-2), buf[3] = a(k-1)
       for the current k (starting with k = 4) */
    seq_t buf[4] = {init[0], init[1], init[2], init[3]};

    idx_t i;
    for (i = 4; i <= n; ++i) {
        /* Indices used for the recurrence:
           a(i-4) -> buf[(i-4)&3]
           a(i-3) -> buf[(i-3)&3]
           a(i-2) -> buf[(i-2)&3]   (the newest term in the window)   */
        seq_t next = buf[(i - 2) & 3] + 2 * buf[(i - 3) & 3] + buf[(i - 4) & 3];

        /* Shift the window: the new value becomes the most recent term */
        buf[i & 3] = next;
    }

    /* After the loop, a(n) is stored in the position corresponding to n mod 4 */
    return buf[n & 3];
}
