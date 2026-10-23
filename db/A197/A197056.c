/*
 * A197056  Number of nX3 0..4 arrays with each element equal to the number
 *          its horizontal and vertical neighbors equal to 2.
 * https://oeis.org/A197056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+2*a(n-4)+a(n-5)+a(n-6) for n >= 7; a(1)=1, a(2)=3,
 *            a(3)=6, a(4)=10, a(5)=14, a(6)=22
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 43, u64 n <= 87, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed initial values */
    static const seq_t init[6] = {1, 3, 6, 10, 14, 22};

    if (n <= 6)
        return init[n - 1];

    /* Use a circular buffer of the last six terms */
    seq_t last[6];
    last[0] = init[0];
    last[1] = init[1];
    last[2] = init[2];
    last[3] = init[3];
    last[4] = init[4];
    last[5] = init[5];

    /* Iterate from 7 up to n */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = last[5] + 2 * last[2] + last[1] + last[0];
        /* Shift the buffer: drop the oldest term */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = next;
    }

    return last[5];
}
