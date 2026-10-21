/*
 * A204716  Number of (n+1) X 2 0..1 arrays with the permanents of 2 X 2
 *          subblocks nondecreasing rightwards and downwards.
 * https://oeis.org/A204716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 9*a(n-3) + 4*a(n-4) + 5*a(n-5) -
 *            2*a(n-6), with a(1)=16, a(2)=51, a(3)=156, a(4)=450, a(5)=1267,
 *            a(6)=3490
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function uses only arithmetic operators defined for seq_t. */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    static const seq_t base[] = {0, 16, 51, 156, 450, 1267, 3490};

    if (n <= 6)
        return base[n];

    /* Maintain the last six values. */
    seq_t prev[7] = {0};
    for (idx_t i = 1; i <= 6; ++i)
        prev[i] = base[i];

    seq_t cur = 0;
    for (idx_t i = 7; i <= n; ++i) {
        cur = 4 * prev[6] - prev[5] - 9 * prev[4] + 4 * prev[3] + 5 * prev[2] - 2 * prev[1];
        /* Shift the window: prev[1] becomes prev[2], etc. */
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = prev[6];
        prev[6] = cur;
    }
    return cur;
}
