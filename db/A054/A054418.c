/*
 * A054418  Number of connected 3 X n binary matrices with rightmost column
 *          [1,1,1]', divided by 4.
 * https://oeis.org/A054418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 3*a(n-2) + 5*a(n-3) Initial values: a(1) = 1,
 *            a(2) = 8, a(3) = 53 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* This sequence is defined for n >= 1.  Behaviour for n == 0 is arbitrary. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* A static table up to a fixed maximum size.  For the problem statement the
       function will be asked for n up to about 10 000, which fits comfortably. */
    enum { MAX_STATIC = 10000 };          /* change if larger values are needed */
    static seq_t cache[MAX_STATIC + 1];   /* cache[0] unused, cache[1] holds a(1) */
    static idx_t max_cached = 0;          /* largest n for which cache[n] is valid */

    /* Initialise the first three entries lazily.  This keeps the function
       order‑independent – calls can arrive in any order. */
    if (max_cached < 1 && n >= 1) {
        cache[1] = (seq_t)1;
        max_cached = 1;
    }
    if (max_cached < 2 && n >= 2) {
        cache[2] = (seq_t)8;
        max_cached = 2;
    }
    if (max_cached < 3 && n >= 3) {
        cache[3] = (seq_t)53;
        max_cached = 3;
    }

    /* Extend the cache up to n using the recurrence. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] * (seq_t)7;
        val -= cache[i - 2] * (seq_t)3;
        val += cache[i - 3] * (seq_t)5;
        cache[i] = val;
    }

    max_cached = n;
    return cache[n];
}
