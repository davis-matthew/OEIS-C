/*
 * A026898  a(n) = Sum_{k=0..n} (n-k+1)^k.
 * https://oeis.org/A026898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (n-k+1)^k
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Arrays sized for the maximum n (~10000) the problem description mentions. */
    static idx_t computed = (idx_t)-1;          /* highest n already computed */
    static seq_t powers[10002];                /* powers[t] = t^{(n+1)-t} */
    static seq_t cache[10001];                 /* a(n) values */

    /* First call: initialise for a(0) */
    if (computed == (idx_t)-1) {
        computed = 0;
        powers[1] = (seq_t)1;      /* 1^(1-1) = 1 */
        cache[0]  = (seq_t)1;      /* a(0) = 1 */
    }

    /* If the requested n has already been computed, just return it. */
    if (n <= computed)
        return cache[n];

    /* Compute successive terms up to n using the recurrence:
       powers_next[t] = powers[t] * t    for t = 1..m
       powers_next[m+1] = 1
       a(m) = sum_{t=1}^{m+1} powers_next[t]   */
    for (idx_t m = computed + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t t = 1; t <= m; ++t) {
            seq_t old = powers[t];
            seq_t newv = old * (seq_t)t;
            sum += newv;
            powers[t] = newv;
        }
        powers[m + 1] = (seq_t)1;
        sum += powers[m + 1];
        cache[m] = sum;
    }
    computed = n;
    return cache[n];
}
