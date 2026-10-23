/*
 * A169417  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18 * (a(n-1)+a(n-2)+...+a(n-31)) - 171 * a(n-32) + N(n)
 *            where N(0)=1, N(1..31)=2, N(32)=1, N(n>32)=0. Initial values up
 *            to n=31 coincide with A170739
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t buf[32] = {0};          /* circular buffer for last 32 values */
    seq_t cur = 0;

    for (idx_t i = 0; i <= n; ++i) {
        /* Determine N(i) */
        seq_t N;
        if (i == 0)
            N = (seq_t)1;
        else if (i <= 31)
            N = (seq_t)2;
        else if (i == 32)
            N = (seq_t)1;
        else
            N = (seq_t)0;

        /* Sum of the last up to 31 terms */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 31 && i >= k; ++k) {
            sum += buf[(i - k) & 31u];
        }

        /* Apply recurrence */
        seq_t term = sum * (seq_t)18;
        if (i >= 32)
            term -= buf[(i - 32) & 31u];

        cur = term + N;
        buf[i & 31u] = cur;
    }

    return buf[n & 31u];
}
