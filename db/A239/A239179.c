/*
 * A239179  Number of (1+1) X (n+1) 0..2 arrays with no element greater than
 *          all horizontal neighbors or equal to all vertical neighbors.
 * https://oeis.org/A239179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 3*a(n-3) + 3*a(n-4) - 4*a(n-5) +
 *            a(n-6) with initial values: a(1)=6, a(2)=26, a(3)=98, a(4)=378,
 *            a(5)=1512, a(6)=6018
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base[7] = {0, 6, 26, 98, 378, 1512, 6018};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    if (n <= 6) return base[n];          /* exact initial values */

    /* Sliding window of the last six values needed for the recurrence.  */
    seq_t prev[6];
    prev[0] = base[6];  /* a(k-1) when k = 7  → a(6) */
    prev[1] = base[5];  /* a(k-2)          → a(5) */
    prev[2] = base[4];  /* a(k-3)          → a(4) */
    prev[3] = base[3];  /* a(k-4)          → a(3) */
    prev[4] = base[2];  /* a(k-5)          → a(2) */
    prev[5] = base[1];  /* a(k-6)          → a(1) */

    for (idx_t k = 7; k <= n; ++k) {
        /* Compute the next term using the recurrence. */
        seq_t next = 4*prev[0] - prev[1] + 3*prev[2] + 3*prev[3] - 4*prev[4] + prev[5];
        /* Shift the window: drop the oldest, add the newest. */
        prev[5] = prev[4];
        prev[4] = prev[3];
        prev[3] = prev[2];
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = next;
    }

    return prev[0];   /* prev[0] now holds a(n) */
}
