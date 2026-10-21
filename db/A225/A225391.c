/*
 * A225391  Expansion of 1/(1 - x - x^2 - x^6 + x^8).
 * https://oeis.org/A225391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-6) - a(n-8) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=8, a(6)=14, a(7)=23
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 44, u64 n <= 88, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* we store up to 10000; this satisfies the typical limit of the tests
       and keeps the implementation simple and portable to C11 and C++17. */
    static seq_t cache[10001] = {0};
    static idx_t cached_len = 0;   /* 0 .. cached_len-1 are valid entries */

    /* initialise the first eight entries once */
    if (cached_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)8;
        cache[6] = (seq_t)14;
        cache[7] = (seq_t)23;
        cached_len = 8;
    }

    /* compute missing values up to n (if any) */
    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t val = cache[i - 1];          /* a(i-1) */
        if (i >= 2) val += cache[i - 2];   /* + a(i-2) */
        if (i >= 6) val += cache[i - 6];   /* + a(i-6) */
        if (i >= 8) val -= cache[i - 8];   /* - a(i-8) */
        cache[i] = val;
    }

    if (n + 1 > cached_len)
        cached_len = n + 1;

    return cache[n];
}
