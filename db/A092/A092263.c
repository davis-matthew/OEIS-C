/*
 * A092263  a(1)=1, a(n+1)=ceiling(phi*a(n))+1 if a(n) is odd,
 *          a(n+1)=ceiling(phi*a(n)) if a(n) is even, where phi=(1+sqrt(5))/2.
 * https://oeis.org/A092263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=6, a(4)=10, a(5)=17,
 *            a(n)=2*a(n-1)-a(n-2)+a(n-3)-a(n-5) for n>5
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Fixed maximum size sufficient for test ranges (e.g., n≈10000). */
    static seq_t cache[20001];
    static idx_t len = 0;
    static int initialized = 0;

    if (!initialized) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)10;
        cache[5] = (seq_t)17;
        len = 5;
        initialized = 1;
    }

    if (n <= len)
        return cache[n];

    /* Compute terms up to n using the linear recurrence. */
    for (idx_t i = len + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] - cache[i - 2] + cache[i - 3] - cache[i - 5];
    }
    len = n;
    return cache[n];
}
