/*
 * A161478  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+113)^2 = y^2.
 * https://oeis.org/A161478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 226 for n > 6 Initial values: a(1)=0,
 *            a(2)=52, a(3)=175, a(4)=339, a(5)=615, a(6)=1312
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Static cache is big enough for all required indices (n ≤ 10000) */
    static seq_t cache[10001];
    static idx_t max_n = 0;    /* highest n for which cache[n] is valid */

    /* Initialise cache with the first six terms on the first call */
    if (max_n == 0)
    {
        cache[1] = 0;
        cache[2] = 52;
        cache[3] = 175;
        cache[4] = 339;
        cache[5] = 615;
        cache[6] = 1312;
        max_n = 6;
    }

    /* If the desired term is already computed, return it */
    if (n <= max_n)
        return cache[n];

    /* Extend the cache up to the requested index using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        cache[i] = 6 * cache[i - 3] - cache[i - 6] + (seq_t)226;
    }
    max_n = n;

    return cache[n];
}
