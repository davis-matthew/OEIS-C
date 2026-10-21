/*
 * A200766  Number of 0..n arrays x(0..5) of 6 elements with nondecreasing
 *          average value.
 * https://oeis.org/A200766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑6 arrays with elements in 0..n such that
 *            the sequence of running averages is nondecreasing (i.e. for each
 *            k, (x0+…+xk)/(k+1) ≤ (x0+…+xk+1)/(k+2))
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 72, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_N 10000
#define MAX_SUM (MAX_N * 6)

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Static arrays large enough for the maximum n. */
    static seq_t cur[MAX_SUM + 2];
    static seq_t next[MAX_SUM + 2];
    static seq_t diff[MAX_SUM + 3];

    idx_t maxSum = 6 * n;

    /* initialise length‑1 arrays */
    for (idx_t i = 0; i <= maxSum; ++i) cur[i] = 0;
    for (idx_t i = 0; i <= n; ++i) cur[i] = 1;
    idx_t curMaxSum = n;

    /* build sequences of lengths 2…6 */
    for (idx_t step = 0; step < 5; ++step)
    {
        idx_t len   = step + 1;            /* current length */
        idx_t nextMaxSum = (len + 1) * n;  /* maximum sum after adding the next element */

        /* clear difference array for the current step */
        for (idx_t i = 0; i <= nextMaxSum + 1; ++i) diff[i] = 0;

        /* range updates for all current sums */
        for (idx_t s = 0; s <= curMaxSum; ++s)
        {
            seq_t ways = cur[s];
            if (ways == 0) continue;

            /* lower bound for the next element */
            idx_t L = (s + len - 1) / len;   /* ceil(s / len) */
            if (L > n) continue;

            idx_t lowSum  = s + L;
            idx_t highSum = s + n;

            diff[lowSum] += ways;
            if (highSum + 1 <= nextMaxSum)
                diff[highSum + 1] -= ways;   /* unsigned subtraction wraps mod 2ⁿ */
        }

        /* build next array from the difference array */
        seq_t acc = 0;
        for (idx_t i = 0; i <= nextMaxSum; ++i)
        {
            acc += diff[i];
            next[i] = acc;
        }

        /* prepare for the next iteration */
        for (idx_t i = 0; i <= nextMaxSum; ++i) cur[i] = next[i];
        curMaxSum = nextMaxSum;
    }

    /* sum all sequences of length 6 */
    seq_t total = 0;
    for (idx_t i = 0; i <= curMaxSum; ++i) total += cur[i];
    return total;
}
