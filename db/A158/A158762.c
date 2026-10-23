/*
 * A158762  Expansion of (1+9*x+80*x^2)/(1-x^2-729*x^3).
 * https://oeis.org/A158762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 729*a(n-3) Initial values: a(0)=1, a(1)=9,
 *            a(2)=81 (OEIS A158762)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cache up to 10000 terms for speed; 10000 is enough for the test limits. */
    static seq_t cache[10001];
    static idx_t cached = (idx_t)-1;   /* -1 means not yet initialized */

    /* Initialize the cache on first call. */
    if (cached == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)81;
        cached = 2;
    }

    /* Return from cache if possible. */
    if (n <= cached) return cache[n];

    /* If n is within the cached range, compute forward storing results. */
    if (n <= 10000) {
        for (idx_t i = cached + 1; i <= n; ++i) {
            cache[i] = cache[i-2] + cache[i-3] * (seq_t)729;
        }
        cached = n;
        return cache[n];
    }

    /* For n > 10000, fall back to an iterative computation without storing all terms. */
    seq_t a0 = cache[0];  /* a(n-3) */
    seq_t a1 = cache[1];  /* a(n-2) */
    seq_t a2 = cache[2];  /* a(n-1) */
    seq_t a_n = 0;        /* placeholder for the current term */

    /* Handle the trivial cases that lie outside the 0..2 range. */
    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = a1 + a0 * (seq_t)729;   /* a(i) = a(i-2) + 729*a(i-3) */
        a0 = a1;                      /* shift window for next iteration */
        a1 = a2;
        a2 = a_n;
    }
    return a_n;
}
