/*
 * A050232  a(n) is the number of n-tosses having a run of 4 or more heads for
 *          a fair coin (i.e., probability is a(n)/2^n).
 * https://oeis.org/A050232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2^n - f(n); f(0) = 1, f(1) = 2, f(2) = 4, f(3) = 8 f(n) =
 *            f(n-1) + f(n-2) + f(n-3) + f(n-4) for n ≥ 4
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No memoisation is required – a single O(n) pass suffices for n ≤ 10000. */
seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;

    /* compute f(n), the number of binary strings of length n with no run of 4 heads */
    if (n == 0)
        return 0;                 /* offset 1 – not used */

    /* base values for n = 1,2,3 */
    if (n <= 3) {
        /* f(1)=2, f(2)=4, f(3)=8 */
        seq_t fbase[4] = {1, 2, 4, 8};
        seq_t f_n = fbase[n];
        return pow2 - f_n;
    }

    /* sliding window for the recurrence f(n)=f(n-1)+f(n-2)+f(n-3)+f(n-4) */
    seq_t f0 = 1;   /* f(0) */
    seq_t f1 = 2;   /* f(1) */
    seq_t f2 = 4;   /* f(2) */
    seq_t f3 = 8;   /* f(3) */
    seq_t fn = 0;

    for (idx_t i = 4; i <= n; ++i) {
        fn = f0 + f1 + f2 + f3;   /* compute f(i) */
        /* shift window */
        f0 = f1;
        f1 = f2;
        f2 = f3;
        f3 = fn;
    }

    return pow2 - fn;
}
