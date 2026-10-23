/*
 * A188213  Number of nondecreasing arrangements of 6 numbers in -(n+4)..(n+4)
 *          with sum zero.
 * https://oeis.org/A188213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + 2*a(n-5) - a(n-6) - a(n-7)
 *            + 2*a(n-8) - a(n-10) - 2*a(n-11) + 3*a(n-12) - a(n-13) with
 *            a(1)=338, a(2)=676, a(3)=1242, a(4)=2137, a(5)=3486, a(6)=5444,
 *            a(7)=8196, a(8)=11963, a(9)=17002, a(10)=23612, a(11)=32134,
 *            a(12)=42955, a(13)=56512
 * Range:     u8 none, u16 n <= 13, u32 n <= 171, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* maximum n expected for the tests; pick a safe upper bound */
    enum { MAX_N = 20000 };

    /* static cache of computed values; 1‑based indexing */
    static seq_t cache[MAX_N + 1];
    static idx_t cached_max = 0;               /* largest index with a value */

    /* base values for n = 1 … 13 */
    static const seq_t base_vals[13] = {
        338, 676, 1242, 2137, 3486,
        5444, 8196, 11963, 17002, 23612,
        32134, 42955, 56512
    };

    /* initialise base values if not already done */
    if (cached_max < 13) {
        for (idx_t i = 1; i <= 13 && i <= MAX_N; ++i) {
            cache[i] = base_vals[i - 1];
        }
        if (cached_max < 13) cached_max = 13;
    }

    /* boundary protection – for safety we ignore n larger than MAX_N */
    if (n == 0 || n > MAX_N) return (seq_t)0;

    /* return cached value if already computed */
    if (n <= cached_max) return cache[n];

    /* compute the missing terms up to n using the recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        cache[i] =
            3 * cache[i - 1]
          - 2 * cache[i - 2]
          -     cache[i - 3]
          + 2 * cache[i - 5]
          -     cache[i - 6]
          -     cache[i - 7]
          + 2 * cache[i - 8]
          -     cache[i -10]
          - 2 * cache[i -11]
          + 3 * cache[i -12]
          -     cache[i -13];
    }
    cached_max = n;
    return cache[n];
}
