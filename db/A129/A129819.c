/*
 * A129819  Antidiagonal sums of triangular array T: T(j,k) = (k+1)/2 for odd
 *          k, T(j,k) = 0 for k = 0, T(j,k) = j+1-k/2 for even k > 0; 0 <= k
 *          <= j.
 * https://oeis.org/A129819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=3, a(5)=4, a(6)=7, *
 *            a(n)=a(n-1)+a(n-2)-a(n-3)+a(n-4)-a(n-5)-a(n-6)+a(n-7) for n>6
 * Range:     u8 n <= 37, u16 n <= 591, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    static const seq_t base[7] = {0, 0, 1, 1, 3, 4, 7};

    if (n < 7)
        return base[n];

    /* Keep the last 7 values in a circular buffer */
    seq_t last[7];
    for (int i = 0; i < 7; ++i)
        last[i] = base[i];

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute the next term using only the last 7 terms */
        seq_t next =
            last[6] + last[5]         /* a(n-1)+a(n-2) */
            - last[4]                 /* -a(n-3) */
            + last[3]                 /* +a(n-4) */
            - last[2] - last[1] + last[0]; /* -a(n-5)-a(n-6)+a(n-7) */
        /* Shift the buffer: discard the oldest term */
        for (int k = 0; k < 6; ++k)
            last[k] = last[k + 1];
        last[6] = next;
    }

    return last[6];
}
