/*
 * A131246  Row sums of triangle A131245.
 * https://oeis.org/A131246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+3*a(n-2)-a(n-3)-a(n-4) * Initial values: a(0)=1,
 *            a(1)=3, a(2)=6, a(3)=13
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Static cache large enough for the expected problem size (≈10 000). */
    static seq_t cache[20001] = {0};
    static idx_t cache_len = 0;  /* number of values already computed */

    /* Initialise the first four values on the first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)13;
        cache_len = 4;
    }

    /* If the requested index is already available, return it. */
    if (n < cache_len)
        return cache[n];

    /* Compute values up to n using the recurrence relation. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i-1] + (cache[i-2] + cache[i-2] + cache[i-2])
                     - cache[i-3] - cache[i-4];
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
